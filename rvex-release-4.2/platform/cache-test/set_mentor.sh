#echo "Hostname is $(hostname)" 
#if [[ $(hostname) != "zeppelin" ]]; then 
#user="agerichsen" 
export PATH=$PATH:/home/tansini/Modelsim/modeltech/linux_x86_64/ 
export LM_LICENSE_FILE=/home/tansini/Modelsim/license.dat
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/tansini/Modelsim/modeltech/lib32/ 
export LD_PRELOAD=/home/tansini/Modelsim/modeltech/lib32/libfreetype.so #elif [[ $(hostname) == "zeppelin" ]]; then user="aerichsen" export #PATH=$PATH:/home/$user/modelsim/modeltech/linux_x86_64/ export #LM_LICENSE_FILE=/home/$user/modelsim/license.dat export #LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/$user/modelsim/modeltech/#lib32/ export LD_PRELOAD=/home/$user/modelsim/modeltech/lib32/#libfreetype.so 
#fi
