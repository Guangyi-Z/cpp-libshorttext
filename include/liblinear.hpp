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

    int ll_get_solver_type()
    {
        return model_->param.solver_type;
    }

    void ll_load_model(std::string model_file)
    {
        if((model_=load_model(model_file.c_str()))==0)
        {
            fprintf(stderr,"can't open model file %s\n",model_file.c_str());
            exit(1);
        }
    }
    void ll_destroy_model()
    {
        free_and_destroy_model(&model_);
        if (!x) {
            free(x);
        }
    }
}

#endif // _LIBLINEAR_HPP_
