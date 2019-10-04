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


#include "volume.hpp"

#include "image/image.hpp"
#include "transfert_function.hpp"
#include "lib/common/error_handling.hpp"
#include "lib/3d/vec3.hpp"

#include <fstream>
#include <cmath>
#include <algorithm>
#include <sstream>


namespace cpe
{

image mip(volume const& vol)
{
    //*************************************//
    //TO DO: Completer MIP
    //*************************************//

    image im(vol.size_y(),vol.size_z());

    im.fill(color(1,1,0));
    float i = 0.0f;
    for(int ky = 0; ky < vol.size_y() ; ++ky){
        for(int kz = 0; kz < vol.size_z(); ++kz){
            for(int kx = 0; kx < vol.size_x() ; ++kx){
                if(vol(kx,ky, kz) > i)
                    i = vol(kx,ky, kz);
            }

              im(ky,kz) = i;
              i = 0.0f;

        }


    }

    return im;
}

image ray_cast(volume const& vol)
{
    //*************************************//
    //TO DO: Completer ray-casting
    //*************************************//

    image im(vol.size_y(),vol.size_z());
    im.fill(color(0,1,1));

    float fn;

    float dx = 1.0f/(vol.size_x()-1);
    color attec ;
    color somme;


    for(int ky = 0; ky < vol.size_y(); ++ky){
        for(int kz = 0; kz < vol.size_z(); ++kz){
            somme = color(0.,0.,0.);
            for(int kx = 0; kx < vol.size_x(); ++kx){
                fn = vol(kx, ky, kz);
                attec = tf_attenuation(fn);
                somme += somme * (1 - attec) * dx + tf_emission(fn) * fn * dx;
            }

            im(ky,kz) = somme ;


        }
    }

    return im;
}


/** Check if given indes is within the size of the image */
static bool is_inside_data(int kx,int ky,int kz,volume const& vol);

/** Assert that the indices are within the image size */
static void assert_indices_inside_data(int kx,int ky,int kz,volume const& vol);



volume::volume():
    size_x_data(0),size_y_data(0),size_z_data(0),data()
{}

void volume::resize(int size_x_param,int size_y_param,int size_z_param)
{
    ASSERT_CPE(size_x_param>=0 && size_y_param>=0 && size_z_param>=0,"Incorrect resize value");
    ASSERT_CPE(size_x_param*size_y_param*size_z_param<800*800*800,"Too large size");

    size_x_data = size_x_param;
    size_y_data = size_y_param;
    size_z_data = size_z_param;

    data.resize(size_x_param*size_y_param*size_z_param);
}



float volume::operator()(int const kx,int const ky,int const kz) const
{
    assert_indices_inside_data(kx,ky,kz,*this);

    int const offset = kx + size_x_data * (ky + size_y_data*kz);

    ASSERT_CPE(offset<static_cast<int>(data.size()),"Critical error");
    return data[offset];
}
float& volume::operator()(int const kx,int const ky,int const kz)
{
    assert_indices_inside_data(kx,ky,kz,*this);

    int const offset = kx + size_x_data * (ky + size_y_data*kz);

    ASSERT_CPE(offset<static_cast<int>(data.size()),"Critical error");
    return data[offset];
}

float volume::get_data(int const kx,int const ky,int const kz) const
{
    return (*this)(kx,ky,kz);
}

float volume::interpolate(float x,float y,float z) const
{
    return trilinear_interpolation(x,y,z);
}








static bool is_inside_data(int const kx,int const ky,int const kz,volume const& vol)
{
    if( kx < vol.size_x()
            && ky < vol.size_y()
            && kz < vol.size_z())
        return true;
    else
        return false;
}

static void assert_indices_inside_data(int const kx,int const ky,int const kz,volume const& vol)
{
    if(is_inside_data(kx,ky,kz,vol)==false)
    {
        std::stringstream message;
        message<<"volume::get_data("<<kx<<","<<ky<<","<<kz<<") are outside volume limits ["<<vol.size_x()<<","<<vol.size_y()<<","<<vol.size_z()<<"]";
        throw exception_cpe(message.str(),EXCEPTION_PARAMETERS_CPE);
    }
}





int volume::size_x() const {return size_x_data;}
int volume::size_y() const {return size_y_data;}
int volume::size_z() const {return size_z_data;}



image slice_x(volume const& vol , int const kx)
{
    ASSERT_CPE( kx<vol.size_x() , "Incorrect slice index");

    int const Ny = vol.size_y();
    int const Nz = vol.size_z();
    image slice(Ny,Nz);

    for(int ky=0 ; ky<Ny ; ++ky)
    {
        for(int kz=0 ; kz<Nz ; ++kz)
        {
            slice(ky,kz) = vol(kx,ky,kz);
        }
    }
    return slice;
}
image slice_y(volume const& vol , int const ky)
{
    ASSERT_CPE( ky<vol.size_y() , "Incorrect slice index");

    int const Nx = vol.size_x();
    int const Nz = vol.size_z();
    image slice(Nx,Nz);

    for(int kx=0 ; kx<Nx ; ++kx)
    {
        for(int kz=0 ; kz<Nz ; ++kz)
        {
            slice(kx,kz) = vol(kx,ky,kz);
        }
    }
    return slice;
}
image slice_z(volume const& vol , int const kz)
{
    ASSERT_CPE( kz<vol.size_z() , "Incorrect slice index");

    int const Nx = vol.size_x();
    int const Ny = vol.size_y();
    image slice(Nx,Ny);

    for(int kx=0 ; kx<Nx ; ++kx)
    {
        for(int ky=0 ; ky<Ny ; ++ky)
        {
            slice(kx,ky) = vol(kx,ky,kz);
        }
    }
    return slice;
}




volume rotated_x(volume const& vol , float const angle)
{
    int const N = std::max(vol.size_y(),vol.size_z());
    int const Nx = vol.size_x();

    volume vol2;
    vol2.resize(Nx,N,N);

    float const cos_a = cos(angle);
    float const sin_a = sin(angle);

#pragma omp parallel for shared(vol2)
    for(int kx=0 ; kx<Nx ; ++kx)
    {
        for(int ky=0 ; ky<N ; ++ky)
        {
            for(int kz=0 ; kz<N ; ++kz)
            {

                float const x0 = static_cast<float>(kx);
                float const y0 = (ky-N/2.0f)*cos_a - (kz-N/2.0f)*sin_a + N/2.0f;
                float const z0 = (ky-N/2.0f)*sin_a + (kz-N/2.0f)*cos_a + N/2.0f;

                float const value = vol.interpolate(x0,y0,z0);
                vol2(kx,ky,kz) = value;
            }
        }
    }

    return vol2;
}

volume rotated_y(volume const& vol , float angle)
{
    int const N = std::max(vol.size_x(),vol.size_z());
    int const Ny = vol.size_y();

    volume vol2;
    vol2.resize(N,Ny,N);

    float const cos_a = cos(angle);
    float const sin_a = sin(angle);

#pragma omp parallel for shared(vol2)
    for(int kx=0 ; kx<N ; ++kx)
    {
        for(int ky=0 ; ky<Ny ; ++ky)
        {
            for(int kz=0 ; kz<N ; ++kz)
            {

                float const x0 = (kx-N/2.0f)*cos_a - (kz-N/2.0f)*sin_a + N/2.0f;
                float const y0 = static_cast<float>(ky);
                float const z0 = (kx-N/2.0f)*sin_a + (kz-N/2.0f)*cos_a + N/2.0f;

                float const value = vol.interpolate(x0,y0,z0);
                vol2(kx,ky,kz) = value;
            }
        }
    }

    return vol2;
}

volume rotated_z(volume const& vol , float angle)
{
    int const N = std::max(vol.size_x(),vol.size_y());
    int const Nz = vol.size_z();

    volume vol2;
    vol2.resize(N,N,Nz);

    float const cos_a = cos(angle);
    float const sin_a = sin(angle);

#pragma omp parallel for shared(vol2)
    for(int kx=0 ; kx<N ; ++kx)
    {
        for(int ky=0 ; ky<N ; ++ky)
        {
            for(int kz=0 ; kz<Nz ; ++kz)
            {
                float const x0 = (kx-N/2.0f)*cos_a - (ky-N/2.0f)*sin_a + N/2.0f;
                float const y0 = (kx-N/2.0f)*sin_a + (ky-N/2.0f)*cos_a + N/2.0f;
                float const z0 = static_cast<float>(kz);

                float const value = vol.interpolate(x0,y0,z0);
                vol2(kx,ky,kz) = value;
            }
        }
    }

    return vol2;
}






void volume::normalize_data()
{
    ASSERT_CPE(data.size()>0 , "No data to normalize");

    //get min and max
    int const N = data.size();
    float value_max = data[0];
    float value_min = data[0];
    for(int k=1 ; k<N ; ++k)
    {
        float const value = data[k];

        if(value_max < value)
            value_max = value;

        if(value_min > value)
            value_min = value;
    }

    float const L = value_max-value_min;

    //rescale data
    for(float& value : data)
        value = (value-value_min)/L;
}



float volume::trilinear_interpolation(float const x,float const y,float const z) const
{
    float const epsilon=1e-5;
    if(x<0 || y<0 || z<0 || x+epsilon>=size_x() || y+epsilon>=size_y() || z+epsilon>=size_z())
    {
        //outside bounds
        return 0.0f;
    }

    int const x0 = static_cast<int>(x);
    int const y0 = static_cast<int>(y);
    int const z0 = static_cast<int>(z);

    float const ux = x-x0;
    float const uy = y-y0;
    float const uz = z-z0;

    float f=0.0f;

    //tri-linear interpolation
    if(x0<size_x()-1 && y0<size_y()-1 && z0<size_z()-1)
    {
        f=   (1-ux)*(1-uy)*(1-uz)*get_data(x0+0,y0+0,z0+0)
            +(1-ux)*(1-uy)*(  uz)*get_data(x0+0,y0+0,z0+1)
            +(1-ux)*(  uy)*(  uz)*get_data(x0+0,y0+1,z0+1)
            +(1-ux)*(  uy)*(1-uz)*get_data(x0+0,y0+1,z0+0)
            +(  ux)*(1-uy)*(1-uz)*get_data(x0+1,y0+0,z0+0)
            +(  ux)*(1-uy)*(  uz)*get_data(x0+1,y0+0,z0+1)
            +(  ux)*(  uy)*(1-uz)*get_data(x0+1,y0+1,z0+0)
            +(  ux)*(  uy)*(  uz)*get_data(x0+1,y0+1,z0+1);
    }
    else
        f = get_data(x0,y0,z0);

    return f;
}

volume volume::resampled_square(int const N) const
{
    volume vol2; vol2.resize(N,N,N);

    for(int k0=0 ; k0<N ; ++k0)
    {
        float const ux = static_cast<float>(k0)/(N-1);
        float const x0 = ux*(size_x()-1);

        for(int k1=0 ; k1<N ; ++k1)
        {
            float const uy = static_cast<float>(k1)/(N-1);
            float const y0 = uy*(size_y()-1);

            for(int k2=0 ; k2<N ; ++k2)
            {
                float const uz = static_cast<float>(k2)/(N-1);
                float const z0 = uz*(size_z()-1);

                vol2(k0,k1,k2) = get_data(x0,y0,z0);
            }
        }
    }

    return vol2;

}







volume volume::smoothed() const
{

    volume v2; v2.resize(size_x(),size_y(),size_z());

    for(int kx=1 ; kx<size_x()-1 ; ++kx)
    {
        for(int ky=1 ; ky<size_y()-1 ; ++ky)
        {
            for(int kz=1 ; kz<size_z()-1 ; ++kz)
            {
                float const f   = get_data(kx  ,ky,kz);

                float const fx0 = get_data(kx-1,ky,kz);
                float const fx2 = get_data(kx+1,ky,kz);

                float const fy0 = get_data(kx,ky-1,kz);
                float const fy2 = get_data(kx,ky+1,kz);

                float const fz0 = get_data(kx,ky,kz-1);
                float const fz2 = get_data(kx,ky,kz+1);

                //box smoothing
                float const v = 1/12.0f*(6*f+fx0+fx2+fy0+fy2+fz0+fz2);

                v2(kx,ky,kz)=v;
            }
        }
    }
    return v2;
}

void volume::load(std::string const& filename)
{

    std::ifstream stream(filename,std::ifstream::in);

    if(!stream.good())
    {
        std::string message="Error in volume::load_v4d("+filename+") : cannot open file \n";
        throw exception_cpe(message,MACRO_EXCEPTION_PARAMETER);
    }

    int Nx = 0;
    int Ny = 0;
    int Nz = 0;

    //read size
    stream >> Nx;
    stream >> Ny;
    stream >> Nz;

    std::cout<<" find file "<<filename<<" size=("<<Nx<<","<<Ny<<","<<Nz<<")"<<std::endl;


    volume vol;
    vol.resize(Nx,Ny,Nz);

    //Read all data
    float value = 0.0f;
    for(int kx=0 ; kx<Nx ; ++kx)
    {
        for(int ky=0 ; ky<Ny ; ++ky)
        {
            for(int kz=0 ; kz<Nz ; ++kz)
            {
                stream >> value;

                if(stream.good())
                    vol(kx,ky,kz) = value;
                else
                    throw exception_cpe("Something wrong in reading file "+filename,EXCEPTION_PARAMETERS_CPE);
            }
        }
    }
    stream.close();

    std::cout<<" file "+filename+" read [OK]"<<std::endl;

    *this = vol;
}

void volume::save(std::string const& filename) const
{
    std::ofstream stream(filename.c_str(),std::ofstream::out);

    if(!stream.good())
    {
        std::string message="Error in volume::save_v4d("+filename+") : cannot open file \n";
        throw exception_cpe(message,EXCEPTION_PARAMETERS_CPE);
    }

    //write the sizes
    stream << size_x_data << " ";
    stream << size_y_data << " ";
    stream << size_z_data << " ";
    stream << std::endl;

    //write data
    for(int kx=0 ; kx<size_x_data ; ++kx)
    {
        for(int ky=0 ; ky<size_y_data ; ++ky)
        {
            for(int kz=0 ; kz<size_z_data ; ++kz)
            {
                float const value = get_data(kx,ky,kz);

                if(!stream.good())
                    throw exception_cpe("Something wrong in writing file "+filename,EXCEPTION_PARAMETERS_CPE);

                stream << value << std::endl;
            }
        }
    }
    stream.close();
}

}

