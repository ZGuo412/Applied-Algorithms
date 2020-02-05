#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "inverter.h"
int main(int argc, char** argv){
	if(argc != 9){
		exit(-1);
	}
	FILE* fp;
	double time_constraint = atof(argv[1]);
	char* file_inverter = argv[2];
	char* file_wire = argv[3];
	char* file_topo = argv[4];
	char* output1 = argv[5];
	char* output2 = argv[6];
	char* output3 = argv[7];
	char* output4 = argv[8];
	double input_c;
	double output_c;
	double output_r;
	double unit_r;
	double unit_c;
	double delay = 0.0;
	fp = fopen(file_inverter,"r");
	fscanf(fp, "%le %le %le\n", &input_c, &output_c, &output_r);
	fclose(fp);									////// complete reading 1st file

	fp = fopen(file_wire, "r");
	fscanf(fp, "%le %le", &unit_r, &unit_c);
	fclose(fp);                                 ////// complete reading 2nd file
	Tree* tree =get_the_tree(file_topo, output1);
	fp = fopen(output1, "w");
	fclose(fp);
	fp = fopen(output1, "a");
	out1(tree, fp);
	fclose(fp);
	fp = fopen(output2,"w");
	fclose(fp);
	get_cap(tree, -1,output_c, unit_c);
	fp = fopen(output2,"ab");
	write_to_binary(tree, fp, output_r, unit_r, delay);
	fclose(fp);
	add_inverter(tree, output_r, unit_r, output_c, time_constraint,unit_c, input_c);
	fp = fopen(output3,"w");
	out3(tree,fp);
	fclose(fp);
	fp = fopen(output4,"w");
	fclose(fp);
	fp = fopen(output4, "ab");
	out4(tree,fp);
	fclose(fp);
	free_tree(tree);
	return EXIT_SUCCESS;
}
