#ifndef _LIBSHORTTEXT_HPP_
#define _LIBSHORTTEXT_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <float.h>
#include <cmath>
#include "liblinear.hpp"

#define NUM_TO_STR( x ) static_cast< std::ostringstream & >( \
    ( std::ostringstream() << std::dec << x ) ).str()

using std::string;
using std::stringstream;
using std::vector;
using std::map;

using liblinear::x;
using liblinear::model_;
using liblinear::max_nr_attr;

namespace libshorttext {

    // options
    string learner_ops, liblinear_ops;
    int is_binary = 1, is_norm2 = 1, is_tf = 0, is_tfidf = 0;

    // libshorttext model variables
    vector<string> idx2cls;
    map<string,int> tok2idx;
    map<string, int> feat2idx;

    void _parse_options(string ops)
    {
        stringstream ss(ops);
        string op;
        int val;
        while(ss >> op >> val) {
            std::cout << op + " " << val << std::endl;
            if(op[0] != '-') {
                fprintf(stderr,"unknown option: -%s\n", op.c_str());
                exit(1);
            }
            switch(op[1])
            {
                case 'D':
                    is_binary = val;
                    break;
                case 'T':
                    is_tf = val;
                    break;
                case 'I':
                    is_tfidf = val;
                    break;
                case 'N':
                    is_norm2 = val;
                    break;
                default:
                    fprintf(stderr,"unknown option: -%c\n", op[1]);
                    exit(1);
            }
        }
    }

    string _bigram(int l, int r)
    {
        return NUM_TO_STR(l) + "," + NUM_TO_STR(r);
    }

    void lst_destroy_model()
    {
        idx2cls.clear();
        tok2idx.clear();
        feat2idx.clear();
    }

    void lst_load_model(string model_path) {
        string classmap_path = model_path + "/class_map.txt";
        string featgen_path = model_path + "/feat_gen.txt";
        string textprep_path = model_path + "/text_prep.txt";
        string options_path = model_path + "/options.txt";

        std::ifstream options_ifs(options_path.c_str());
        std::getline(options_ifs, learner_ops);
        std::getline(options_ifs, liblinear_ops);
        std::cout<< "options: " << learner_ops+"; "+liblinear_ops << std::endl;
        _parse_options(learner_ops);

        std::ifstream classmap_ifs(classmap_path.c_str());
        string cls;
        while (std::getline(classmap_ifs, cls)) {
            idx2cls.push_back(cls);
        }

        std::ifstream textprep_ifs(textprep_path.c_str());
        string token;
        int idx2tok = 0;
        while (std::getline(textprep_ifs, token)) {
            tok2idx[token] = idx2tok ++;
        }

        std::ifstream featgen_ifs(featgen_path.c_str());
        string ngram;
        int idx2feat = 0;
        while (std::getline(featgen_ifs, ngram)) {
            string::size_type idx = ngram.find('\t');
            if (idx != string::npos) {
                // bigram
                stringstream ss(ngram);
                int l,r;
                ss >> l >> r;
                feat2idx[_bigram(l,r)] = idx2feat;
            }
            else {
                // unigram
                feat2idx[NUM_TO_STR(ngram)] = idx2feat;
            }

            idx2feat ++;
        }
    }

    vector<string> lst_text2tok(string text, char sep)
    {
        vector<string> v;
		stringstream ss;
		ss.str(text);
		string item;
		while (std::getline(ss, item, sep)) {
            v.push_back(item);
        }

        return v;
    }

    vector<int> _tok2index(vector<string> tokens)
    {
        vector<int> id;
        for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            if (tok2idx.end() != tok2idx.find(*it)) {
                id.push_back(tok2idx[*it]);
            }
            // todo
            // ignore unseen token in train
        }

        return id;
    }

    map<int,int> _tok2feat(vector<int> tokens)
    {
        map<int,int> feat;
        // unigram
        for(vector<int>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            string ft = NUM_TO_STR(*it);
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
        for(vector<int>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
            if (it+1 >= tokens.end()) break;
            string ft = _bigram(*it, *(it+1));
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

    // void _normalize(struct feature_node* x, int binary, int norm, int tf, int idf, double* idf_val)
    void _normalize(struct feature_node* x)
    {
        struct feature_node* xi;

        if (is_binary) {
            xi = x;
            while(xi->index != -1)
            {
                xi->value = xi->value != 0;
                ++xi;
            }
        }

        if(is_tf)
        {
            double norm = 0;
            xi = x;
            while(xi->index != -1)
            {
                norm += xi->value;
                ++xi;
            }

            xi = x;
            if(norm != 0)
                while(xi->index != -1)
                {
                    xi->value /= norm;
                    ++xi;
                }
        }

        /*todo
        if(is_tfidf)
        {
            xi = x;
            while(xi->index != -1)
            {
                xi->value *= idf_val[xi->index-1];
                ++xi;
            }
        }
        */

        if(is_norm2)
        {
            double norm = 0;
            xi = x;
            while(xi->index != -1)
            {
                norm += xi->value * xi->value;
                ++xi;
            }

            norm = sqrt(norm);

            xi = x;
            if(norm != 0)
                while(xi->index != -1)
                {
                    xi->value /= norm;
                    ++xi;
                }
        }
    }

    struct feature_node* _feat2node(map<int,int> feats)
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

        for(map<int,int>::iterator it = feats.begin(); it != feats.end(); ++it) {
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

        _normalize(x);

        return x;
    }

    string lst_predict(vector<string> tokens)
    {
		double predict_label;
        vector<int> tokidxs = _tok2index(tokens);
        map<int,int> feats = _tok2feat(tokidxs);
        predict_label = predict(model_, _feat2node(feats));

        return idx2cls[predict_label];
    }
}
#endif // _LIBSHORTTEXT_HPP_
