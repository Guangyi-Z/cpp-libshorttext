#ifndef _CALCULATOR_HPP_
#define _CALCULATOR_HPP_

// #include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "linear.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace libshorttext {

    int get_liblinear_version() {
        return liblinear_version;
    }

    void read_model(std::string model_path) {
        std::string classmap_path = model_path + "/class_map.txt";
        std::string featgen_path = model_path + "/feat_gen.txt";
        std::string options_path = model_path + "/options.txt";
        std::string textprep_path = model_path + "/text_prep.txt";

        std::ifstream textprep_ifs(textprep_path.c_str());
        std::string token;
        std::vector<std::string> tok2idx;
        while (std::getline(textprep_ifs, token)) {
             tok2idx.push_back(token);
        }
        // for(std::vector<std::string>::iterator it = tok2idx.begin(); it != tok2idx.end(); ++it) {
        for(std::vector<std::string>::iterator it = tok2idx.begin(); it != tok2idx.begin() + 10; ++it) {
            std::cout << *it << std::endl;
        }
    }

    struct model* model_;
    struct feature_node *x = NULL;
    int max_nr_attr = 64;

    void liblinear_load_model(std::string model_file)
    {
        if((model_=load_model(model_file.c_str()))==0)
        {
            fprintf(stderr,"can't open model file %s\n",model_file.c_str());
            exit(1);
        }
    }
    void liblinear_destroy_model()
    {
	    free_and_destroy_model(&model_);
        if (x) {
	        free(x);
        }
    }

    struct feature_node* liblinear_svm2feat(const char* line)
    {
        if (!x) {
            x = (struct feature_node *) malloc(max_nr_attr*sizeof(struct feature_node));
        }

        int nr_class=get_nr_class(model_);
        int j, n;
        int nr_feature=get_nr_feature(model_);
        if(model_->bias>=0)
            n=nr_feature+1;
        else
            n=nr_feature;

		int i = 0;
		double target_label;
		char *idx, *val, *label, *endptr;
		int inst_max_index = 0; // strtol gives 0 if wrong format

		label = strtok((char *)line," \t\n");
		if(label == NULL) // empty line
	        fprintf(stderr,"Wrong input format\n");

		target_label = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
	        fprintf(stderr,"Wrong input format\n");

		while(1)
		{
			if(i>=max_nr_attr-2)	// need one more for index = -1
			{
				max_nr_attr *= 2;
				x = (struct feature_node *) realloc(x,max_nr_attr*sizeof(struct feature_node));
			}

			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;
			errno = 0;
			x[i].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x[i].index <= inst_max_index)
	            fprintf(stderr,"Wrong input format\n");
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
	            fprintf(stderr,"Wrong input format\n");

			// feature indices larger than those in training are not used
			if(x[i].index <= nr_feature)
				++i;
		}

		if(model_->bias>=0)
		{
			x[i].index = n;
			x[i].value = model_->bias;
			i++;
		}
		x[i].index = -1;

        return x;
    }

	double liblinear_predict(std::string text)
    {
		double predict_label;
        predict_label = predict(model_, liblinear_svm2feat(text.c_str()));

        return predict_label;
    }
}
#endif // _CALCULATOR_HPP_
