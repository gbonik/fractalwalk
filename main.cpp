#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <cstdlib>

#include "fractal_laplacian.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        cout << "Usage: " << argv[0] << " P PATH OUTPUT_FILE" << endl;
        return 1;
    }

    double p = atof(argv[1]);
    const char* path_str = argv[2];
    const char* output_filename = argv[3];

    if (p <= 0.0 || p >= 1.0 || p == 0.5)
    {
        cout << "P must be in (0, 1), excluding 0.5" << endl;
        return 1;
    }

    int level = strlen(path_str) - 1;
    vector<int> path;
    for (int i = 0; i <= level; ++i)
    {
        if (!(path_str[i] >= '0' && path_str[i] <= '2'))
        {
            cout << "PATH must be a string of digits 0, 1, 2" << endl;
            return 1;
        }
        path.push_back(path_str[i] - '0');
    }

    if (path[0] == 2)
    {
        cout << "The first digit in PATH must be 0 or 1" << endl;
        return 1;
    }

    for (int i = 1; i <= level; ++i)
        if (path[i] != path[0])
        {
            if (path[i] == 2)
            {
                cout << "Invalid PATH: if it starts with 0, the first non-zero digit must be 1; "
                    "if it starts with 1, the first digit different from 1 must be 0." << endl;
                return 1;
            }
            break;
        }

    ofstream out(output_filename);
    if (!out)
    {
        cout << "Failed to open " << output_filename << " for writing" << endl;
        return 1;
    }

    int len = 1;
    for (int i = 0; i < level; ++i)
        len *= 3;
    len++;

    vector<double> func(len);
    double lambda = compute_eigenpair(p, level, &path[0], &func[0]);

    cout << "Level " << level << "; eigenvalue is " << lambda << endl;
    out << scientific;

    for (int i = 0; i < len; ++i)
    {
        double x = double(i) / double(len - 1);
        out << x << ' ' << func[i] << endl;
    }

    cout << "Output written to " << output_filename << endl;
    return 0;
}




