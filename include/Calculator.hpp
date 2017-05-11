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
#include <sstream>
#include <vector>
#include <map>

#define NUM_TO_STR( x ) static_cast< std::ostringstream & >( \
    ( std::ostringstream() << std::dec << x ) ).str()


namespace libshorttext {

    struct model* model_;
    struct feature_node *x = NULL;
    int max_nr_attr = 64;

    int get_liblinear_version() {
        return liblinear_version;
    }

    std::string bigram(int l, int r)
    {
        return NUM_TO_STR(l) + "," + NUM_TO_STR(r);
    }

    std::vector<std::string> idx2tok;
    std::map<std::string,int> tok2idx;
    std::map<std::string, int> feat2idx;
    void read_model(std::string model_path) {
        std::string classmap_path = model_path + "/class_map.txt";
        std::string featgen_path = model_path + "/feat_gen.txt";
        std::string options_path = model_path + "/options.txt";
        std::string textprep_path = model_path + "/text_prep.txt";

        std::ifstream textprep_ifs(textprep_path.c_str());
        std::string token;
        while (std::getline(textprep_ifs, token)) {
             idx2tok.push_back(token);
        }
        for(std::vector<std::string>::iterator it = idx2tok.begin(); it != idx2tok.end(); ++it) {
            tok2idx[*it] = it - idx2tok.begin();
        }

        std::ifstream featgen_ifs(featgen_path.c_str());
        std::string ngram;
        int idx2feat = 0;
        while (std::getline(featgen_ifs, ngram)) {
            std::string::size_type idx = ngram.find('\t');
            if (idx != std::string::npos) {
                // bigram
                std::stringstream ss(ngram);
                int l,r;
                ss >> l >> r;
                feat2idx[bigram(l,r)] = idx2feat;
            }
            else {
                // unigram
                feat2idx[NUM_TO_STR(ngram)] = idx2feat;
            }

            idx2feat ++;
        }
    }

    std::vector<std::string> text2tok(std::string text, char sep)
    {
        std::vector<std::string> v;
		std::stringstream ss;
		ss.str(text);
		std::string item;
		while (std::getline(ss, item, sep)) {
            v.push_back(item);
        }

        return v;
    }

    std::vector<int> tok2index(std::vector<std::string> tokens)
    {
        std::vector<int> id;
        for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            if (tok2idx.end() != tok2idx.find(*it)) {
                id.push_back(tok2idx[*it]);
            }
            // todo
            // ignore unseen token in train
        }

        return id;
    }

    std::map<int,int> tok2feat(std::vector<int> tokens)
    {
        std::map<int,int> feat;
        // unigram
        for(std::vector<int>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            std::string ft = NUM_TO_STR(*it);
            if (feat2idx.end() != feat2idx.find(ft)) {
                int id = feat2idx[ft];
                if (feat.end() != feat.find(id)) {
                    feat[id] ++;
                }
                else {
                    feat[id] = 1;
                }
            }
        }
        // bigram
        for(std::vector<int>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            if (it+1 >= tokens.end()) break;
            std::string ft = bigram(*it, *(it+1));
            if (feat2idx.end() != feat2idx.find(ft)) {
                int id = feat2idx[ft];
                if (feat.end() != feat.find(id)) {
                    feat[id] ++;
                }
                else {
                    feat[id] = 1;
                }
            }
        }

        return feat;
    }

    struct feature_node* feat2node(std::map<int,int> feats)
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

        for(std::map<int,int>::iterator it = feats.begin(); it != feats.end(); ++it) {
			if(i>=max_nr_attr-2)	// need one more for index = -1
			{
				max_nr_attr *= 2;
				x = (struct feature_node *) realloc(x,max_nr_attr*sizeof(struct feature_node));
			}
			x[i].index = it->first;
			x[i].value = it->second;

			// feature indices larger than those in training are not used
			if(x[i].index <= nr_feature)
                i++;
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

	double liblinear_predict(std::vector<std::string> tokens)
    {
		double predict_label;
        std::vector<int> tokidxs = tok2index(tokens);
        std::map<int,int> feats = tok2feat(tokidxs);
        predict_label = predict(model_, feat2node(feats));

        return predict_label;
    }
}
#endif // _CALCULATOR_HPP_
