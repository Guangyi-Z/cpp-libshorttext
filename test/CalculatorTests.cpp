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

    GIVEN( "liblinear" ) {
        WHEN( "liblinear" ) {
            THEN( "version" ) {
                REQUIRE( get_liblinear_version() == 211 );
            }
            THEN( "predict" ) {
                liblinear_load_model("../../test/stub/train_file.model/learner/liblinear_model");

                std::ifstream test_ifs("../../test/stub/test_file.svm");
                std::string line;
                std::vector<double> py;
                while (std::getline(test_ifs, line)) {
                    py.push_back(liblinear_predict(line));
                }

                liblinear_destroy_model();

                std::ofstream output_ofs("../../test/stub/predict_result");
                for(std::vector<double>::iterator it = py.begin(); it != py.end(); ++it) {
                    output_ofs << *it << std::endl;
                }
            }
            THEN( "tokenize" ) {
                std::string model_path = "../../test/stub/train_file.model_converted";
                read_model(model_path);

                // std::string text = "Jewelry & Watches	multicolor inlay sterling silver post earrings jewelry";
                std::string text = "multicolor inlay sterling silver post earrings jewelry";
                char sep = ' ';
                std::vector<int> tokens = text2tok(text, sep);

                for(std::vector<int>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
                    std::cout << *it << std::endl;
                }
                std::cout << "### count: " << tokens.size() << std::endl;
            }
            THEN( "load model" ) {
                std::string model_path = "../../test/stub/train_file.model_converted";
                read_model(model_path);
            }
        }
    }
}
