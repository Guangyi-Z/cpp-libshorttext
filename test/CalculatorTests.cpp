#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Calculator.hpp"
#include <unistd.h> // getcwd()

using namespace libshorttext;

std::string get_working_path() {
    const int MAXPATHLEN_ = 2048;
    char temp[MAXPATHLEN_];
    return ( getcwd(temp, MAXPATHLEN_) ? std::string( temp ) : std::string("") );
}

SCENARIO( "Libshorttext", "[libshorttext]" ) {

    GIVEN( "libshorttext_converted" ) {
        THEN( "read model and proprecessing" ) {
            std::string model_path = "../../test/stub/train_file.model_converted";
            clear_model();
            read_model(model_path);

            REQUIRE( tok2idx[">>dummy<<"] == 0 );
            REQUIRE( tok2idx["calgary"] == 1 );
            REQUIRE( tok2idx["rawlins"] == 11069 );
            REQUIRE( feat2idx[">>dummy<<"] == 0 );
            REQUIRE( feat2idx["1"] == 1 );
            REQUIRE( feat2idx["3877,11069"] == 47244 );

            // std::string text = "Jewelry & Watches	multicolor inlay sterling silver post earrings jewelry";
            std::string text = "multicolor inlay sterling silver post earrings jewelry";
            char sep = ' ';
            std::vector<int> tokidxs = tok2index(text2tok(text, sep));
            for(std::vector<int>::iterator it = tokidxs.begin(); it != tokidxs.end(); ++it) {
                std::cout << *it << std::endl;
            }

            std::map<int,int> feats = tok2feat(tokidxs);
            for(std::map<int,int>::iterator it = feats.begin(); it != feats.end(); ++it) {
                std::cout << it->first << " => " << it->second << std::endl;
            }
        }
        THEN( "text2tok" ) {
            std::string text = "夏天 是 最好 的季节 一饱眼福";
            char sep = ' ';
            std::vector<std::string> tokens = text2tok(text, sep);
            REQUIRE( tokens[0] == "夏天" );
            REQUIRE( tokens[4] == "一饱眼福" );
        }
    }

    GIVEN( "liblinear" ) {
        THEN( "version" ) {
            REQUIRE( get_liblinear_version() == 211 );
        }
        THEN( "predict" ) {
            std::string model_path = "../../test/stub/train_file.model_converted";
            clear_model();
            read_model(model_path);

            liblinear_load_model("../../test/stub/train_file.model/learner/liblinear_model");

            std::ifstream test_ifs("../../test/stub/test_file.text");
            std::string line;
            std::vector<std::string> py;
            char sep = ' ';
            while (std::getline(test_ifs, line)) {
                std::vector<std::string> tokens = text2tok(line, sep);
                py.push_back(liblinear_predict(tokens));
            }

            std::vector<std::string> y;
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

            liblinear_destroy_model();
        }
    }
}
