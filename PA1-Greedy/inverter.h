#include <stdio.h>
#include <stdlib.h>
#define buff 256
typedef struct _Tree{
	int sink;
	double cap;
	double left_distance;
	double  right_distance;
	struct _Tree *left;
	struct _Tree *right;
	double total_cap;
	int num_inv;
}Tree;


void out1(Tree* tree, FILE* fp);
Tree* get_the_tree(char* file_topo, char* output1);
int get_lines(char* file_topo);
Tree* construct(FILE* fp);
double get_cap(Tree* tree,double length, double outc,double unitc);
void write_to_binary(Tree* tree, FILE* fp, double output_r, double unit_r, double delay);
void add_inverter(Tree* tree, double output_r, double unit_r, double output_c, double time_con, double unit_c, double input_c);
void out3(Tree* tree, FILE*fp);
void out4(Tree* tree, FILE*fp);
void free_tree(Tree* tree);

