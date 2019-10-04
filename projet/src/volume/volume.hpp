/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef VOLUME_HPP
#define VOLUME_HPP

#include <iostream>
#include <vector>


namespace cpe
{

class image;
class color;




/** Container of a scalar volume data set */
class volume
{
public:

    volume();

    //*********************************************//
    // I/O
    //*********************************************//

    /** Load a .4d volume file */
    void load(std::string const& filename);
    /** Save a .4d volume file */
    void save(std::string const& filename) const;

    /** Set the values to be between 0 and 1 */
    void normalize_data();


    //*********************************************//
    // Size
    //*********************************************//

    /** Resize (fill with zero) */
    void resize(int size_x_param,int size_y_param,int size_z_param);

    /** \brief get size_x */
    int size_x() const;
    /** \brief get size_y */
    int size_y() const;
    /** \brief get size_z */
    int size_z() const;


    //*********************************************//
    //*********************************************//
    // Get/Set value
    //*********************************************//
    //*********************************************//

    /** Get the value at position (kx,ky,kz) */
    float operator()(int kx,int ky,int kz) const;
    /** Get the reference value at position (kx,ky,kz) */
    float& operator()(int kx,int ky,int kz);


    /** \brief get the value at any position (x,y,z) using intepolation */
    float interpolate(float x,float y,float z) const;


    //*********************************************//
    //*********************************************//
    // Data modification
    //*********************************************//
    //*********************************************//

    /** \brief resample data to fit into a cube of a given size */
    volume resampled_square(int N) const;

    /** \brief smoothing */
    volume smoothed() const;





private:

    /** \brief internal tri-linear interpolation of the value at position (x,y,z). Return 0 if out-of-bounds*/
    float trilinear_interpolation(float x,float y,float z) const;


    /** \brief get the value at position (kx,ky,kz) */
    float get_data(int kx,int ky,int kz) const;

    /** Size in x direction*/
    int size_x_data;
    /** Size in y direction*/
    int size_y_data;
    /** Size in z direction*/
    int size_z_data;
    /** Internal data as concatenated vector of float */
    std::vector <float> data;
};

//*********************************************//
// Rotation
//*********************************************//

/** Rotate the volume data along the x axis */
volume rotated_x(volume const& vol , float angle);

/** Rotate the volume data along the y axis */
volume rotated_y(volume const& vol , float angle);

/** Rotate the volume data along the z axis */
volume rotated_z(volume const& vol , float angle);


//*********************************************//
// Slicing
//*********************************************//

/** Extract a slice of data in plane x=kx.
 * \return an image of size Ny*Nz */
image slice_x(volume const& vol , int kx);

/** Extract a slice of data in plane y=ky.
 * \return an image of size Nx*Nz */
image slice_y(volume const& vol , int ky);

/** Extract a slice of data in plane z=kz.
 * \return an image of size Nx*Ny */
image slice_z(volume const& vol , int kz);

//*********************************************//
// Visualization
//*********************************************//

/** Apply MIP projection along the x direction (return black & white image of size Ny*Nz) */
image mip(volume const& vol);

/** Apply classical ray-casting projection along the x direction (return color texture of size 3*Ny*Nz) */
image ray_cast(volume const& vol);

}

#endif
