import tensorflow as tf
import numpy as np

# Define the model using tf.keras
class LinearModel(tf.keras.Model):
    def __init__(self):
        super(LinearModel, self).__init__()
        self.dense = tf.keras.layers.Dense(1, name='dense')

    def call(self, inputs):
        return self.dense(inputs)

# Create the model
model = LinearModel()

# Compile the model
model.compile(optimizer=tf.keras.optimizers.SGD(learning_rate=0.01),
              loss='mean_squared_error')

# Define dummy input and output data
x_train = np.array([[[1]], [[2]], [[3]], [[4]]], dtype=np.float32)
y_train = np.array([[[1]], [[2]], [[3]], [[4]]], dtype=np.float32)

# Train the model
model.fit(x_train, y_train, epochs=10)

# Save the model in the SavedModel format with input signatures
tf.saved_model.save(model, "saved_model/linear_model", 
                    signatures={'serving_default': model.call.get_concrete_function(tf.TensorSpec(shape=[None, 1, 1], dtype=tf.float32))})

print("Model saved in the SavedModel format.")
