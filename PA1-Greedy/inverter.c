#include "inverter.h"

void out1(Tree* tree, FILE* fp){
	if(tree  == NULL){
		return;
	}
	else{
		if(tree->left_distance >= 0){
			fprintf(fp,"(%.10le %.10le)\n", tree->left_distance, tree->right_distance);
			out1(tree->left, fp);
			out1(tree->right,fp);
		}
		else{
			fprintf(fp,"%d(%.10le)\n", tree->sink, tree->cap);
			return;
		}
	}
}
void free_tree(Tree* tree){
	if(tree == NULL){
		return;
	}
	else{
		free_tree(tree->right);
		free_tree(tree->left);
		free(tree);
	}
}
Tree* get_the_tree(char* file_topo, char* output1){
	int count = get_lines(file_topo);	
	char** data = (char **)malloc(sizeof(char*) *count);
	for(int i = 0; i < count; i++){
		data[i] = (char*)malloc(sizeof(char) * buff);
	}
	for(int i = 0; i < count; i++){
		for(int j = 0; j < buff; j++){
			data[i][j] = '\0';
		}
	}
	FILE*fp = fopen(file_topo, "r");
	for(int i = 0; i < count; i++){
		fgets(data[i], buff, fp);
	}
	fclose(fp);
	FILE* fp1 = fopen(output1, "w");
	int temp = count;
	while(count >0){
		fputs(data[count - 1], fp1);
		count--;
	}
	fclose(fp1);
	for(int i = 0; i < temp; i++){
		free(data[i]);
	}
	free(data);
	FILE* fp2 = fopen(output1, "r");
	Tree* tree = construct(fp2);
	fclose(fp2);
	return tree;
}
Tree* construct(FILE* fp){
	if(feof(fp)){
		return NULL;
	}
	int sink;
	double cap, left_dis, right_dis;
	if(fscanf(fp, "%d(%le)", &sink, &cap)){
		Tree* node = (Tree*)malloc(sizeof(Tree));
		node->left_distance = node->right_distance = node->total_cap =-1;
		node->sink = sink;
		node->cap = cap;
		node->num_inv = 0;
		node->left = node->right = NULL;
		return node;
	}
	else if(fscanf(fp, "(%le %le)", &left_dis, &right_dis)){
		Tree* temp = (Tree*)malloc(sizeof(Tree));
		temp->left = temp->right = NULL;
		temp->left_distance = left_dis;
		temp->right_distance = right_dis;
		temp->cap = -1;
		temp->sink = -1;
		temp->num_inv = 0;
		temp->total_cap = -1;
		temp->right = construct(fp);
		temp->left = construct(fp);
		return temp;
	}
	return NULL;
}

int get_lines(char* file_topo){
	FILE* fp = fopen(file_topo, "r");
	int ans = 0;
	char *temp = malloc(sizeof(*temp) * buff);
	while(fgets(temp, buff, fp)){
		ans++;
	}
	free(temp);
	fclose(fp);
	return ans;
}


double get_cap(Tree* tree, double length, double outc, double unitc){
	if(length <= 0){
		if(tree->left_distance >=0){
			tree->total_cap = (tree->left_distance + tree->right_distance) * unitc / 2.0 + outc;
		}
		else{
			tree->total_cap = tree->cap;
			return tree->total_cap;
		}
	}
	else{
		if(tree->left_distance >=0){
			tree->total_cap = (tree->left_distance + tree->right_distance + length) * unitc / 2.0;
		}
		else{
			tree->total_cap = length * unitc/2.0 + tree->cap;
			return tree->total_cap;
		}
	}
	tree->total_cap += get_cap(tree->left,tree->left_distance,outc, unitc);
	tree->total_cap += get_cap(tree->right,tree->right_distance, outc, unitc);
	return tree->total_cap;	
}


void write_to_binary(Tree* tree, FILE* fp, double output_r, double unit_r, double delay){
	if(tree->left != NULL){
		delay += output_r * tree->total_cap;
		write_to_binary(tree->left, fp, unit_r * tree->left_distance, unit_r, delay);
		write_to_binary(tree->right, fp, unit_r * tree->right_distance, unit_r, delay);
	}
	else{
		delay += output_r * tree->total_cap;
		fwrite(&tree->sink, sizeof(int), 1, fp);
		printf("%le\n", delay);
		fwrite(&delay, sizeof(double),1,fp);
	}
	return;
}


void add_inverter(Tree* tree, double output_r, double unit_r, double output_c, double time_con, double unit_c, double input_c){
	if(tree!= NULL){
		if(tree->left == NULL){
			return;
		}
//		printf("%le\n",tree->left->cap);
		tree->cap = 0;
		if(tree->left->cap >= 0){
			if(tree->left->cap == 0){
				tree->left->total_cap += tree->left_distance * unit_c / 2;
				tree->right->total_cap += tree->left_distance * unit_c / 2;
			}
			double delay = tree->left_distance * unit_r * tree->left->total_cap + tree->right_distance * unit_r *tree->right->total_cap;
			if(delay < time_con){
				tree->num_inv = 1;
				tree->cap = tree->num_inv * input_c;
//				if(tree->cap >= 0){printf("%leaabbcc\n", tree->cap);}
			}
			else{
				delay -=time_con;
				while(delay > time_con){
					tree->num_inv++;
					tree->total_cap = tree->left->total_cap + tree->right->total_cap + (tree->left_distance + tree->right_distance) * unit_c /2.0 + output_c * tree->num_inv;
					delay-=tree->total_cap * output_r / tree->num_inv;
				}
			}
			tree->total_cap = tree->num_inv * input_c;
			if(tree->num_inv % 2 != 0 && tree->num_inv != 1){
				tree->num_inv++;
			}
			return;
		}
		else{
			add_inverter(tree->left, output_r, unit_r, output_c, time_con, unit_c, input_c);
			add_inverter(tree->right,output_r, unit_r, output_c, time_con, unit_c, input_c);
			tree->left->total_cap += tree->left_distance * unit_c / 2;
			tree->right->total_cap += tree->left_distance * unit_c / 2;
			double delay = tree->left_distance * unit_r * tree->left->total_cap + tree->right_distance * unit_r *tree->right->total_cap;
			if(delay < time_con){
				tree->num_inv = 1;
				tree->cap = tree->num_inv * input_c;
//				if(tree->cap >= 0){printf("%leaabbcc\n", tree->cap);}
			}
			else{
				delay -=time_con;
				while(delay > time_con){
					tree->num_inv++;
					tree->total_cap = tree->left->total_cap + tree->right->total_cap + (tree->left_distance + tree->right_distance) * unit_c /2.0 + output_c * tree->num_inv;
					delay-=tree->total_cap * output_r / tree->num_inv;
				}
			}
			if(tree->num_inv % 2 != 0 && tree->num_inv != 1){
				tree->num_inv++;
			}
			tree->total_cap = tree->num_inv * input_c;
			return;
		}
	}
	return;
}


void out3(Tree* tree, FILE* fp){
	if(tree == NULL){
		return;
	}
	if(tree->left_distance >=0){
		out3(tree->left, fp);
		out3(tree->right,fp);
		fprintf(fp,"(%.10le %.10le %d)\n", tree->left_distance, tree->right_distance, tree->num_inv);
	}
	else{
		fprintf(fp,"%d(%.10le)\n", tree->sink, tree->cap);
	}
	return;

}

void out4(Tree* tree, FILE* fp){
	if(tree == NULL){
		return;
	}
	if(tree->left_distance >= 0){
		out4(tree->left, fp);
		out4(tree->right,fp);
		int non_leaf = -1;
		fwrite(&non_leaf, sizeof(int),1,fp);
		fwrite(&tree->left_distance,sizeof(double),1,fp);
		fwrite(&tree->right_distance,sizeof(double),1,fp);
		fwrite(&tree->num_inv, sizeof(int),1,fp);
	}
	else{
		fwrite(&tree->sink,sizeof(int),1,fp);
		fwrite(&tree->cap, sizeof(double),1,fp);
	}
	return;
}
