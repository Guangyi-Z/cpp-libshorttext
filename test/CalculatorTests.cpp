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
            THEN( "train" ) {
                char* argv[] = {"exe", "../../thirdparity/liblinear/heart_scale", "./heart_scale.model"};
                liblinear_train(2, argv);
                // REQUIRE( get_liblinear_version() == 211 );
            }
            THEN( "load model" ) {
                std::string model_path = "../../test/stub/train_file.model_converted";
                read_model(model_path);
            }
        }
    }
}
