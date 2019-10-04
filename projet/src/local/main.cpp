
/** TP 5ETI IMI- CPE Lyon - 2015-2016 */



#include "lib/common/error_handling.hpp"
#include "image/image.hpp"
#include "volume/volume.hpp"
#include "volume/transfert_function.hpp"

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>


using namespace cpe;

static std::string get_string(int k);

int main(int argc,char *argv[])
{

    std::cout<<"***********************"<<std::endl;
    std::cout<<"run "<<argv[0]<<" with "<<argc-1<<" parameters ... \n"<<std::endl;


    try
    {
        //************************//
        //Load
        //************************//
        volume vol; vol.load("data/head_ct_50.4d");
        vol.normalize_data();


        //exemple de recuperation de donnees
        float value=vol(15,20,25);
        std::cout<<"\nTest: Valeur des donnees en (15,20,25)="<<value<<std::endl;



        //************************//
        //PARTIE 1: SLICES
        //************************//


//        //************************//
//        //export slices
//        //************************//
//        image sx = slice_x ( vol , vol.size_x()/2 );
//        sx.save("output/slice_x.ppm");

//        image sy = slice_y ( vol , vol.size_y()/2 );
//        sy.save("output/slice_y.ppm");

//        image sz = slice_z ( vol , vol.size_z()/2 );
//        sz.save("output/slice_z.ppm");


//        //export all the slices along the z direction
//        // comment it to save times when not used
//        for(int k=0 ; k<vol.size_z() ; ++k)
//        {
//            std::cout<<k<<","<<vol.size_z()<<std::endl;

//            image const sz_slice = slice_z(vol,k);
//            sz_slice.save("output/slice_z_"+get_string(k)+".ppm");
//        }





        //************************//
        //PARTIE 2: ROTATION+MIP
        //************************//

//        //number of rotation steps
//        int const N_rotation=20;

//        for(int counter=0 ; counter<N_rotation ; ++counter)
//        {
//            std::string const number = get_string(counter);

//            std::cout<<"export file "<<counter+1<<" / "<<N_rotation<<std::endl;

//            //Rotation
//            float const angle = (2.0f*M_PI*counter) / N_rotation + M_PI/2.0f;
//            volume vol2 = rotated_z(vol,angle);

//            //MIP
//            image val = mip(vol2);

//            val.save("output/mip_"+number+".ppm");
//        }



        //************************//
        //PARTIE 3: RAY-CASTING
        //************************//

//        //export transfert function
//        export_tranfert_function("output/transfert_function.dat");

//        for(int counter=0 ; counter<N_rotation ; ++counter)
//        {
//            std::string const number=get_string(counter);

//            std::cout<<"export file "<<counter+1<<" / "<<N_rotation<<std::endl;

//            //Rotation
//            float const angle = (M_PI*2.0f*counter) / N_rotation + M_PI/2.0f;
//            volume vol2 = rotated_z(vol,angle);

//            //Ray Casting
//            image val_couleur=ray_cast(vol2);

//            val_couleur.save("output/ray_"+number+".ppm");
//        }

    }
    catch(cpe::exception_cpe const& e)
    {


        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"Exception found"<<std::endl;
        std::cout<<e.info()<<std::endl;
        std::cout<<"\nSaving backtrace in file backtrace_log.txt"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;


        std::ofstream back_file("backtrace_log.txt");
        back_file<<e.info()<<std::endl;
        back_file<<e.info_backtrace();
        back_file.close();

        exit(1);
    }


    std::cout<<"Exit Main"<<std::endl;

    return 0;
}

static std::string get_string(int const k)
{

    std::stringstream stream_number;stream_number<<k;
    std::string number(stream_number.str());

    if(k<10)
        number="00"+number;
    else if(k<100)
        number="0"+number;
    return number;

}
