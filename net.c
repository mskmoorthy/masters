//Initial Test

#include "fann.h"
#include <stdio.h>
#include <string.h>

//scales each input and output individually to a range [0,1]
//returns max of each column (for use in reversing scale)
int** scale_data(struct fann_train_data* data){
	int** min_max_vals = (int**)calloc(2, sizeof(int*));
	min_max_vals[0] = (int*)calloc(4, sizeof(int));
	min_max_vals[1] = (int*)calloc(4, sizeof(int));
	int i, j;

	//calculate max/min values in each column
	for(i = 0; i < fann_length_train_data(data); i++){
		for(j = 0; j < 3; j++){
			if(min_max_vals[0][j] > data->input[i][j])
				min_max_vals[0][j] = data->input[i][j];
			else if(min_max_vals[1][j] < data->input[i][j])
				min_max_vals[1][j] = data->input[i][j];
		}
		if(min_max_vals[0][3] > data->output[i][0])
			min_max_vals[0][3] = data->output[i][0];
		else if(min_max_vals[1][3] < data->output[i][0])
			min_max_vals[1][3] = data->output[i][0];
	}

	for(i = 0; i < fann_length_train_data(data); i++){
		data->input[i][0] = (data->input[i][0] - min_max_vals[0][0]) / (float)(min_max_vals[1][0] - min_max_vals[0][0]);
		data->input[i][1] = (data->input[i][1] - min_max_vals[0][1]) / (float)(min_max_vals[1][1] - min_max_vals[0][1]);
		data->input[i][2] = (data->input[i][2] - min_max_vals[0][2]) / (float)(min_max_vals[1][2] - min_max_vals[0][2]);
		data->output[i][0] = (data->output[i][0] - min_max_vals[0][3]) / (float)(min_max_vals[1][3] - min_max_vals[0][3]);
	}
	return min_max_vals;
}

float unscale_output(float output, int** min_max_vals){
	return (output * (min_max_vals[1][3] - min_max_vals[0][3])) + min_max_vals[0][3];
}

int main(){

	const float desired_error = (const float) 0.00001;
	const unsigned int max_epochs = 50000;
	const unsigned int epochs_between_reports = 100;
	const int num_layers = 4;
	const int inputs = 3;
	const int outputs = 1;

	struct fann *ann = fann_create_standard(num_layers, inputs, 4, 5, outputs);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID);
	fann_set_activation_function_output(ann, FANN_SIGMOID);
	//fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);
	//fann_set_training_algorithm(ann, FANN_TRAIN_BATCH);

	struct fann_train_data * data = fann_read_train_from_file("NN_input.data");
	//SCALE DATA
	int** min_max_vals = scale_data(data);

	fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);
	fann_save(ann, "lottery.net");

	fann_print_connections(ann);
	fann_print_parameters(ann);
	printf("TESTING NETWORK:\n\n");

	int i;
	fann_type* calc_out;
	FILE *f = fopen("NN.log", "w");
	if(f == NULL){
		printf("ERROR OPENING FILE\n");
		exit(1);
	}

	for(i = 0; i < fann_length_train_data(data); i++){
		fann_reset_MSE(ann);
		calc_out = fann_run( ann, data->input[i] );
		float US_calc_out = unscale_output(calc_out[0], min_max_vals);
		float US_output = unscale_output(data->output[i][0], min_max_vals);
		//UNSCALE OUTPUT
		fprintf(f, "%8f, %8f\n", US_calc_out, US_output);
	}
	fann_destroy_train(data);
	fann_destroy(ann);
	free(min_max_vals[0]);
	free(min_max_vals[1]);
	free(min_max_vals);
	return 0;	
}