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


#include "transfert_function.hpp"
#include "lib/common/error_handling.hpp"
#include "image/color.hpp"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>

namespace cpe
{

color tf_emission(float const value)
{
    float r = 8.0f*value;
    float b = 8.0f*value;
    float g = 0.0f;
    if(value>0.2f && value<0.9f)
        g = 8.0f*(value-0.2f)/0.7f;

    return color(r,g,b);
}

color tf_attenuation(float const value)
{
    return color(10.0f*value,10.0f*value,10.0f*value);
}


void export_tranfert_function(std::string const& filename)
{
    std::ofstream stream(filename.c_str(),std::ofstream::out);
    if(!stream.good())
        throw exception_cpe("Error in opening file "+filename+"... : cannot open file ",EXCEPTION_PARAMETERS_CPE);

    //compute TF for all values
    int const N=256;
    for(int k=0 ; k<N ; ++k)
    {
        float const f = static_cast<float>(k)/(N-1);

        color attenuation = tf_attenuation(f);
        color emission = tf_emission(f);

        stream<<f<<" "<<attenuation.r()<<" "<<attenuation.g()<<" "<<attenuation.b()<<" "<<emission.r()<<" "<<emission.g()<<" "<<emission.b()<<std::endl;
    }


    stream.close();

}

}
