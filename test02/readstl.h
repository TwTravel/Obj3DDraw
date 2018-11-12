#ifndef READ_STL_STRUCTS_AND_CLASSES_H
#define READ_STL_STRUCTS_AND_CLASSES_H

// define the vec3d struct (a 3D vector with three components)
struct vec3d
{ 
    double x;  /// x-component of the 3D vector
    double y;  /// y-component of the 3D vector
    double z;  /// z-component of the 3D vector
};

// A triangular element is defined using the three vertices and a normal vector defining the orientation
// of the element in relation to the surface of the solid object. The normal vector usually points outwards
// for each surface element.

class triangle
{
public:
    /// 3 components of the normal vector to the triangle
    vec3d normal;
    /// 3 coordinates of the three vertices of the triangle
    vec3d point[3];
};

int read_binary_STL_file(std::string STL_filename,std::vector<triangle> & facet,
                         double & x_min, double & x_max, double & y_min, double & y_max, double & z_min, double & z_max)
{
    // specify the location of STL files on this computer
    std::string STL_files_path = "";   

    // declare an (input) file object
    std::ifstream binaryInputFile;

    // open the STL file by using the full path and the name
    // specify that the file is opened in "read-only" mode
    // specify that the file is opened in binary format
    binaryInputFile.open((STL_files_path + STL_filename).c_str(), std::ifstream::in | std::ifstream::binary);

    // check whether the file was opened successfully
    // if yes then continue otherwise terminate program execution
    if(binaryInputFile.fail())
    {
        std::cout << "ERROR: Input STL file could not be opened!" << std::endl;
        return (1);
    }

    // position the pointer to byte number 80
    binaryInputFile.seekg(80);

    // read the number of facets (triangles) in the STL geometry
    int numberOfTriangles;
    binaryInputFile.read((char*) &numberOfTriangles, sizeof(int));

    // declare an object "tri" of type triangle (see main.h for the definition of the triangle class)
    triangle tri;

    // storage space for the "unused bytes" 
    char unused_bytes[2];

    // initialize parameters that will be used to store the minimum and maximum extents of the geometry
    // described by the STL file
    x_min = 1e+30, x_max = -1e+30;
    y_min = 1e+30, y_max = -1e+30;
    z_min = 1e+30, z_max = -1e+30;

    // temporary floating point variable
    float temp_float_var;

    for(int count=0;count<numberOfTriangles;count++)
    {
        // read the three components of the normal vector
        binaryInputFile.read((char*)&temp_float_var,4); tri.normal.x = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.normal.y = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.normal.z = (double) temp_float_var;

        // read the three coordinates of vertex 1 
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[0].x = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[0].y = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[0].z = (double) temp_float_var;

        // read the three coordinates of vertex 2 
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[1].x = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[1].y = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[1].z = (double) temp_float_var;

        // read the three coordinates of vertex 3
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[2].x = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[2].y = (double) temp_float_var;
        binaryInputFile.read((char*)&temp_float_var,4); tri.point[2].z = (double) temp_float_var;

        // read the 2 unused bytes
        binaryInputFile.read(unused_bytes,2);

        // update geometry extents along X, Y and Z based on vertex 1 of this triangle
        if (tri.point[0].x < x_min) x_min = tri.point[0].x;
        if (tri.point[0].x > x_max) x_max = tri.point[0].x;
        if (tri.point[0].y < y_min) y_min = tri.point[0].y;
        if (tri.point[0].y > y_max) y_max = tri.point[0].y;
        if (tri.point[0].z < z_min) z_min = tri.point[0].z;
        if (tri.point[0].z > z_max) z_max = tri.point[0].z;

        // update geometry extents along X, Y and Z based on vertex 2 of this triangle
        if (tri.point[1].x < x_min) x_min = tri.point[1].x;
        if (tri.point[1].x > x_max) x_max = tri.point[1].x;
        if (tri.point[1].y < y_min) y_min = tri.point[1].y;
        if (tri.point[1].y > y_max) y_max = tri.point[1].y;
        if (tri.point[1].z < z_min) z_min = tri.point[1].z;
        if (tri.point[1].z > z_max) z_max = tri.point[1].z;

        // update geometry extents along X, Y and Z based on vertex 3 of this triangle
        if (tri.point[2].x < x_min) x_min = tri.point[2].x;
        if (tri.point[2].x > x_max) x_max = tri.point[2].x;
        if (tri.point[2].y < y_min) y_min = tri.point[2].y;
        if (tri.point[2].y > y_max) y_max = tri.point[2].y;
        if (tri.point[2].z < z_min) z_min = tri.point[2].z;
        if (tri.point[2].z > z_max) z_max = tri.point[2].z;

        // add data for this triangle to the "facet" vector
        facet.push_back(tri);
    }

    // explicitly close the connection to the input STL file
    binaryInputFile.close();

    return (0);  // all is well
}

int read_ascii_STL_file(std::string STL_filename, std::vector<triangle> & facet,
                        double & x_min, double & x_max, double & y_min, double & y_max, double & z_min, double & z_max)
{
    // specify the location of STL files on this computer
    std::string STL_files_path = "";   

    // declare a (input) file object
    std::ifstream asciiInputFile;

    // open the STL file by using the full path and the name
    // specify that the file is opened in "read-only" mode
    asciiInputFile.open((STL_files_path + STL_filename).c_str(), std::ifstream::in);

    // check whether the file was opened successfully
    // if yes then continue otherwise terminate program execution
    if(asciiInputFile.fail())
    {
        std::cout << "ERROR: Input STL file could not be opened!" << std::endl;
        return(1); // error
    }

    // read in the contents line by line until the file ends

    // initialize counter for counting the number of lines in this file
    int triangle_number = 0;  

    // declare an object "tri" of type triangle (see above for the definition of the triangle struct)
    triangle tri;

    // declare some string objects
    std::string junk;
    std::string string1,string2;

    // read in the first line (until the /n delimiter) and store it in the string object "line"
    getline(asciiInputFile,junk);

    // initialize parameters that will be used to store the minimum and maximum extents of the geometry
    // described by the STL file
    x_min = 1e+30, x_max = -1e+30;
    y_min = 1e+30, y_max = -1e+30;
    z_min = 1e+30, z_max = -1e+30;

    // begin loop to read the rest of the file until the file ends
    while(true)
    {
        // read the components of the normal vector
        asciiInputFile >> string1 >> string2 >> tri.normal.x >> tri.normal.y >> tri.normal.z;        //  1
        // continue reading this line until the \n delimiter
        getline(asciiInputFile,junk);                                                                //  1

        // read the next line until the \n delimiter
        getline(asciiInputFile,junk);                                                                //  2

        // read the (x,y,z) coordinates of vertex 1            
        asciiInputFile >> string1 >> tri.point[0].x >> tri.point[0].y >> tri.point[0].z;             //  3
        getline(asciiInputFile,junk);                                                                //  3

        // read the (x,y,z) coordinates of vertex 2            
        asciiInputFile >> string1 >> tri.point[1].x >> tri.point[1].y >> tri.point[1].z;             //  4
        getline(asciiInputFile,junk);                                                                //  4

        // read the (x,y,z) coordinates of vertex 3            
        asciiInputFile >> string1 >> tri.point[2].x >> tri.point[2].y >> tri.point[2].z;             //  5
        getline(asciiInputFile,junk);                                                                //  5

        // read some more junk
        getline(asciiInputFile,junk);                                                                //  6
        getline(asciiInputFile,junk);                                                                //  7

        // update geometry extents along X, Y and Z based on vertex 1 of this triangle
        if (tri.point[0].x < x_min) x_min = tri.point[0].x;
        if (tri.point[0].x > x_max) x_max = tri.point[0].x;
        if (tri.point[0].y < y_min) y_min = tri.point[0].y;
        if (tri.point[0].y > y_max) y_max = tri.point[0].y;
        if (tri.point[0].z < z_min) z_min = tri.point[0].z;
        if (tri.point[0].z > z_max) z_max = tri.point[0].z;

        // update geometry extents along X, Y and Z based on vertex 2 of this triangle
        if (tri.point[1].x < x_min) x_min = tri.point[1].x;
        if (tri.point[1].x > x_max) x_max = tri.point[1].x;
        if (tri.point[1].y < y_min) y_min = tri.point[1].y;
        if (tri.point[1].y > y_max) y_max = tri.point[1].y;
        if (tri.point[1].z < z_min) z_min = tri.point[1].z;
        if (tri.point[1].z > z_max) z_max = tri.point[1].z;

        // update geometry extents along X, Y and Z based on vertex 3 of this triangle
        if (tri.point[2].x < x_min) x_min = tri.point[2].x;
        if (tri.point[2].x > x_max) x_max = tri.point[2].x;
        if (tri.point[2].y < y_min) y_min = tri.point[2].y;
        if (tri.point[2].y > y_max) y_max = tri.point[2].y;
        if (tri.point[2].z < z_min) z_min = tri.point[2].z;
        if (tri.point[2].z > z_max) z_max = tri.point[2].z;

        // break out of the while loop if "end-of-file" becomes true
        if (asciiInputFile.eof()) break;

        // increment the triangle number
        triangle_number++;

        // add data for this triangle to the "facet" vector
        facet.push_back(tri);
    }
    // end while loop

    // explicitly close the output file
    asciiInputFile.close();

    return (0);   // all is well
}

#endif
