// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/cc/framework/gradients_test.cc
// https://gist.github.com/asimshankar/7c9f8a9b04323e93bb217109da8c7ad2
// https://gist.github.com/asimshankar/5c96acd1280507940bad9083370fe8dc

#include <iostream>
#include <vector>
#include <cstdlib>
#include <tensorflow/cc/saved_model/loader.h>
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/graph/default_device.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/cc/framework/ops.h>
#include <tensorflow/cc/client/client_session.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  const std::string export_dir = "./saved_model/linear_model";
  const std::string checkpoint_dir = "./checkpoints";
  const std::string checkpoint_prefix = "./checkpoints/checkpoint";

  tensorflow::Status status;

  // Load the saved model
  tensorflow::SavedModelBundle bundle;
  status = tensorflow::LoadSavedModel(tensorflow::SessionOptions(), tensorflow::RunOptions(), export_dir, {"serve"}, &bundle);
  if (!status.ok())
  {
    std::cerr << "Error loading model: " << status.ToString() << std::endl;
    return -1;
  }

  tensorflow::Session *session = bundle.GetSession();

  // Now the session is initialized and the variables are restored

  // Print the operations in the graph to find the correct tensor names
  const tensorflow::GraphDef &graph_def = bundle.meta_graph_def.graph_def();
  for (const auto &node : graph_def.node())
  {
    std::cout << "Node name: " << node.name() << std::endl;
  }

  // saved_model_cli show --dir ./saved_model/linear_model --all

  // Assume the input and output tensor names based on the printed nodes
  const std::string input_tensor_name = "serving_default_inputs:0";
  const std::string output_tensor_name = "StatefulPartitionedCall:0";

  // Optionally, you can verify the restoration by running some operations
  // For example, you can run an inference to verify the model
  std::vector<float> batch = {1.0, 2.0, 3.0};
  tensorflow::Tensor input(tensorflow::DT_FLOAT, tensorflow::TensorShape({static_cast<int>(batch.size()), 1, 1}));
  for (size_t i = 0; i < batch.size(); ++i)
  {
    input.flat<float>()(i) = batch[i];
  }

  std::vector<tensorflow::Tensor> outputs;

  status = session->Run({{input_tensor_name, input}}, {output_tensor_name}, {}, &outputs);
  if (!status.ok())
  {
    std::cout << status.ToString() << std::endl;
    return 1;
  }

  // Print the output
  std::cout << "Model output: " << outputs[0].scalar<float>()() << std::endl;

  // Close the session
  session->Close();

  return 0;
}