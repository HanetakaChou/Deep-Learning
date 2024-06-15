```bash
pip3 install tensorflow   

# https://www.tensorflow.org/guide/gpu
env TF_CPP_MAX_VLOG_LEVEL=3 python3 -c "import tensorflow as tf; print(tf.config.list_physical_devices('GPU'))"  

# https://developer.download.nvidia.com/compute/cuda/repos/rhel9/x86_64/
yum install libcudnn8
```
