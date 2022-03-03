/*
jf - JSON formatter
version 0.1
https://github.com/tveerman/jf.git

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2022 Thomas Veerman

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "json.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <vector>

static void
usage()
{
        std::cout
            << "jf [-h] [-i n] [file]" << std::endl
            << "\t-h\tShows this help message." << std::endl
            << "\t-i\tNumber of spaces to indent. Default is 2. Use -1 for no indentation at all."
            << std::endl
            << "\tfile\tPath to file to indent. When no path is provided the data is read "
               "from stdin."
            << std::endl;
        exit(1);
}

static bool
get_data(std::string const &path, nlohmann::json &j)
{
        if (path.empty()) {
                std::cin >> j;
                return true;
        }

        if (std::filesystem::exists(path)) {
                std::ifstream ifs(path);
                if (ifs.good()) {
                        ifs >> j;
                } else {
                        std::cerr << "Could not read from file" << std::endl;
                        return false;
                }
        } else {
                std::cerr << "File does not exist" << std::endl;
                return false;
        }

        return true;
}

int
main(int argc, char *argv[])
{
        bool i_flag = false;
        long long indentation = 2;
        std::string path;

        for (int i = 1; i < argc; i++) {
                std::string arg = argv[i];

                if (i_flag) {
                        indentation = strtoll(argv[i], NULL, 10);
                        i_flag = false;
                        continue;
                }

                if (arg == "-i") {
                        i_flag = true;
                } else if (arg == "--help" || arg == "-h") {
                        usage();
                } else if (path.empty()) {
                        path = arg;
                } else {
                        usage();
                }
        }

        if (indentation < -1 || indentation > 8) {
                indentation = 2;
        }

        nlohmann::json j;
        if (!get_data(path, j)) {
                return 1;
        }

        try {
                std::cout << j.dump(indentation) << std::endl;
                return 0;
        } catch (nlohmann::json::exception &e) {
                std::cerr << "Failed to parse json: " << e.what() << std::endl;
                return 1;
        }
}
