#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "libshorttext.hpp"
#include "util.hpp"

using namespace libshorttext;

SCENARIO( "Libshorttext", "[libshorttext]" ) {

    GIVEN( "libshorttext_converted" ) {
        THEN( "read model and proprecessing" ) {
            string model_path = "../../test/stub/train_file.model_converted";
            lst_destroy_model();
            lst_load_model(model_path);

            REQUIRE( tok2idx[">>dummy<<"] == 0 );
            REQUIRE( tok2idx["calgary"] == 1 );
            REQUIRE( tok2idx["rawlins"] == 11069 );
            REQUIRE( feat2idx[">>dummy<<"] == 0 );
            REQUIRE( feat2idx["1"] == 1 );
            REQUIRE( feat2idx["3877,11069"] == 47244 );

            // string text = "Jewelry & Watches	multicolor inlay sterling silver post earrings jewelry";
            string text = "multicolor inlay sterling silver post earrings jewelry";
            char sep = ' ';
            vector<int> tokidxs = _tok2index(lst_text2tok(text, sep));
            for(vector<int>::iterator it = tokidxs.begin(); it != tokidxs.end(); ++it) {
                std::cout << *it << std::endl;
            }

            map<int,int> feats = _tok2feat(tokidxs);
            for(map<int,int>::iterator it = feats.begin(); it != feats.end(); ++it) {
                std::cout << it->first << " => " << it->second << std::endl;
            }
        }
        THEN( "text2tok" ) {
            string text = "夏天 是 最好 的季节 一饱眼福";
            char sep = ' ';
            vector<string> tokens = lst_text2tok(text, sep);
            REQUIRE( tokens[0] == "夏天" );
            REQUIRE( tokens[4] == "一饱眼福" );
        }
    }

    GIVEN( "liblinear" ) {
        THEN( "version" ) {
            REQUIRE( liblinear::ll_get_version() == 211 );
        }
        THEN( "predict" ) {
            string model_path = "../../test/stub/train_file.model_converted";
            lst_destroy_model();
            lst_load_model(model_path);

            liblinear::ll_load_model(model_path + "/liblinear_model");

            std::ifstream test_ifs("../../test/stub/test_file.text");
            string line;
            vector<string> py;
            char sep = ' ';
            while (std::getline(test_ifs, line)) {
                vector<string> tokens = lst_text2tok(line, sep);
                py.push_back(lst_predict(tokens));
            }

            vector<string> y;
            // std::ifstream result_ifs("../../test/stub/predict_result");
            std::ifstream result_ifs("../../test/stub/predict_result_py");
            while (std::getline(result_ifs, line)) {
                // y.push_back(atof(line.c_str()));
                y.push_back(line);
            }
            REQUIRE(y.size() == py.size());

            int n = 0;
            for(int i = 0; i<py.size(); i++) {
                n += py[i] == y[i]? 1 : 0;
            }
            REQUIRE(n == py.size());

            liblinear::ll_destroy_model();
        }
    }
}
