#ifndef _LIBLINEAR_HPP_
#define _LIBLINEAR_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cerrno>
#include "linear.h"

namespace liblinear {

    struct model* model_;
    struct feature_node *x = NULL;
    int max_nr_attr = 64;

    int ll_get_version()
    {
        return liblinear_version;
    }

    void ll_load_model(std::string model_file)
    {
        if((model_=load_model(model_file.c_str()))==0)
        {
            fprintf(stderr,"can't open model file %s\n",model_file.c_str());
            exit(1);
        }

		// model_->param.solver_type = L2R_L2LOSS_SVC_DUAL
		// model_->param.eps = DBL_MAX;
		// model_->param.C = 1
		// model_->param.p = 0.1
		// model_->param.nr_weight = 0
		// model_->param.weight_label = (int*) Malloc(int, 1);
		// model_->param.weight = (double*) Malloc(double, 1);

		// model_->param.bias = -1
		// model_->param.cross_validation = False
		// model_->param.nr_fold = 0
		// model_->param.print_func = NULL;
    }
    void ll_destroy_model()
    {
        // destroy_param(&(model_->param));
	    free_and_destroy_model(&model_);
        if (x) {
	        free(x);
        }
    }
}

#endif // _LIBLINEAR_HPP_
