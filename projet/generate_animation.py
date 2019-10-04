
import os

def get_all_file_with_name(name,dir_in):

    all_files = os.listdir(dir_in)

    matched_files = []
    for f in all_files : 
        if f.startswith(name) and f.endswith(".ppm"):
            matched_files.append(f[0:-4])
    return sorted(matched_files)

def convert_files(name,dir_in,dir_out,size):

    file_list = os.popen("ls "+dir_in+name+".ppm 2> /dev/null ").read()
    file_list = file_list.replace(".ppm","")
    file_list = file_list.replace(dir_in,"")
    tokens = sorted(file_list.split("\n"))

    for f in tokens:
        if len(f)>1:
            os.system("convert -resize "+str(size)+" "+dir_in+f+".ppm"+" "+dir_out+f+".png")


    # for f in file_list:
    #     os.system("convert -resize 800x800 "+dir_in+f+".ppm "+dir_out+f+".png")

def create_animation(name_in,name_anim,dir_in,dir_out):

    if len(os.popen("ls "+dir_in+name_in+".png 2> /dev/null ").read()) > 0:
        os.system("convert "+dir_in+name_in+".png -loop 0 "+dir_out+name_anim+".gif")



dir_in  = "output/"
dir_out = "output_png/"
dir_anim   = "output_anim/"

if os.path.isdir(dir_out) == False:
    os.mkdir(dir_out)
if os.path.isdir(dir_anim) == False:
    os.mkdir(dir_anim)

convert_files("mip_*",dir_in,dir_out,500)
create_animation("mip_*","mip",dir_out,dir_anim)

convert_files("slice_z_*",dir_in,dir_out,200)
create_animation("slice_z_*","slice",dir_out,dir_anim)

convert_files("ray_*",dir_in,dir_out,500)
create_animation("ray_*","ray",dir_out,dir_anim)
