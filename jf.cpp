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
get_data(std::string const &path, std::vector<char> &data)
{
        if (path.empty()) {
                std::copy(std::istream_iterator<char>(std::cin), std::istream_iterator<char>(),
                          std::back_inserter(data));
                return true;
        }

        if (std::filesystem::exists(path)) {
                std::ifstream ifs(path);
                if (ifs.good()) {
                        std::copy(std::istream_iterator<char>(ifs), std::istream_iterator<char>(),
                                  std::back_inserter(data));
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

static bool
parse_data(std::vector<char> const &data, int indentation)
{
        try {
                nlohmann::json j = nlohmann::json::parse(data.begin(), data.end());
                std::cout << j.dump(indentation) << std::endl;
        } catch (nlohmann::json::exception &e) {
                std::cerr << "Failed to parse json: " << e.what() << std::endl;
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

        std::vector<char> data;
        if (!get_data(path, data)) {
                return 1;
        }

        if (!parse_data(data, (int)indentation)) {
                return 1;
        }

        return 0;
}
