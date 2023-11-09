#!/usr/bin/env python
# coding: utf-8

import pandas as pd
import numpy as np

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

import matplotlib.pyplot as plt

import MLDataLib

def check_linear_size(params_dict, input_size):
    l_in = params_dict['input size']
    l_out = params_dict['output size']
    
    if (input_size[-1] != l_in):
        raise RuntimeError('invalid size: {} != {}'.format(input_size[-1], l_in))
    
    output_size = input_size.copy()
    output_size[-1] = l_out
    
    return output_size

def check_conv1d_size(params_dict, input_size):
    in_depth = params_dict['in depth']
    out_depth = params_dict['out depth']
    
    kernel = params_dict['kernel']
    stride = params_dict['stride']
    padding = params_dict['padding']
    dilitation = params_dict['dilitation']
    
    if (input_size[-1] < kernel): # todo
        raise RuntimeError('invalid size: {} < {}'.format(input_size[-1], kernel))
        
    if (input_size[-2] != in_depth): # todo
        raise RuntimeError('invalid size: {} != {}'.format(input_size[-2], in_depth))

    output_size = input_size.copy()
    output_size[-1] = (input_size[-1] + 2 * padding - dilitation * (kernel - 1) - 1) / stride + 1
    output_size[-2] = out_depth
        
    return output_size

def check_conv2d_size(params_dict, input_size):
    in_depth = params_dict['in depth']
    out_depth = params_dict['out depth']
    
    kernel = params_dict['kernel'] #note [h,w]
    stride = params_dict['stride'] #note [h,w]
    padding = params_dict['padding'] #note [h,w]
    dilitation = params_dict['dilitation'] #note [h,w]
    
    if (input_size[-1] < kernel[1] or input_size[-2] < kernel[0]): # todo
        raise RuntimeError('invalid size: {}x{} < {}x{}'.format(input_size[-1], input_size[-2], kernel[1], kernel[0]))
        
    if (input_size[-3] != in_depth): # todo
        raise RuntimeError('invalid size: {} != {}'.format(input_size[-3], in_depth))

    output_size = input_size.copy()
    output_size[-1] = (input_size[-1] + 2 * padding[1] - dilitation[1] * (kernel[1] - 1) - 1) / stride[1] + 1
    output_size[-2] = (input_size[-2] + 2 * padding[0] - dilitation[0] * (kernel[0] - 1) - 1) / stride[0] + 1
    output_size[-3] = out_depth
    
    return output_size

def check_pool_size(params_dict, input_size):
    kernel = params_dict['kernel']
    stride = params_dict['stride']
    padding = params_dict['padding']
    dilitation = params_dict['dilitation']
    
    if (input_size[-1] < kernel): # todo
        raise RuntimeError('invalid size: {} < {}'.format(input_size, kernel))
        
    output_size = input_size.copy()
    output_size[-1] = (input_size[-1] + 2 * padding - dilitation * (kernel - 1) - 1) / stride + 1
        
    return output_size

def check_pool2d_size(params_dict, input_size):
    kernel = params_dict['kernel'] #note [h,w]
    stride = params_dict['stride'] #note [h,w]
    padding = params_dict['padding'] #note [h,w]
    dilitation = params_dict['dilitation'] #note [h,w]
    
    if (input_size[-1] < kernel[1] or input_size[-2] < kernel[0]): # todo
        raise RuntimeError('invalid size: {}x{} < {}x{}'.format(input_size[-1], input_size[-2], kernel[1], kernel[0]))

    output_size = input_size.copy()
    output_size[-1] = (input_size[-1] + 2 * padding[1] - dilitation[1] * (kernel[1] - 1) - 1) / stride[1] + 1
    output_size[-2] = (input_size[-2] + 2 * padding[0] - dilitation[0] * (kernel[0] - 1) - 1) / stride[0] + 1
    
    return output_size

def check_embedding_size(params_dict, input_size):
    vocab_size = params_dict['vocab size']
    embedding_size = params_dict['embedding size']

    padding = 0 #params_dict[2] if (len(layer_size) > 2) else 0
    
    output_size = input_size.copy()
    output_size.append(embedding_size)
    
    return output_size

def check_reshape_size(params_dict, input_size):
    return params_dict['size']

def check_flatten_size(params_dict, input_size):
    flat_from = params_dict['from']
    flat_to = params_dict['to']
    
    if (flat_from < 0 or len(input_size) < flat_to):
        raise RuntimeError('invalid params: flat_from {}, flat_to{} with input size {}'.format(flat_from, flat_to, input_size))
    
    output_size = []
    for size in input_size[:flat_from]:
        output_size.append(size)
    
    output_size.append(np.prod(input_size[flat_from : flat_to + 1]))
    
    for size in input_size[flat_to + 1:]:
        output_size.append(size)
    
    return output_size

def check_recurrent_size(params_dict, input_size):
    expected_size = params_dict['input_size']
    
    if (input_size[-1] != expected_size):
        raise RuntimeError('invalid params: input_size {} != expected_size {}'.format(input_size[-1], expected_size))
    
    hidden_size = params_dict['hidden_size']
    is_bidirectional = params_dict['bidirectional']
    
    output_size = input_size.copy()
    
    output_size[-1] = hidden_size * (2 if is_bidirectional else 1)

    return output_size

def check_size(layer_name, layer_size, input_size):
    if ('Linear' == layer_name):
        return check_linear_size(layer_size, input_size)
    elif ('Conv1d' == layer_name):
        return check_conv1d_size(layer_size, input_size)
    elif ('Conv2d' == layer_name):
        return check_conv2d_size(layer_size, input_size)
    elif ('Pool' == layer_name):
        return check_pool_size(layer_size, input_size)
    elif ('Pool2d' == layer_name):
        return check_pool2d_size(layer_size, input_size)
    elif ('Embedding' == layer_name):
        return check_embedding_size(layer_size, input_size)
    elif ('Reshape' == layer_name):
        return check_reshape_size(layer_size, input_size)
    elif ('Flatten' == layer_name):
        return check_flatten_size(layer_size, input_size)
    elif ('Recurrent' == layer_name):
        return check_recurrent_size(layer_size, input_size)
    else:
        return input_size

class ReshapeLayer(nn.Module):
    def __init__(self, size):
        super().__init__()
        self.size = size

    def forward(self, x):
        return x.reshape(self.size)

    def string(self):
        return "ReshapeLayer: " + str(self.size)
    
class ConcatinateLayer(nn.Module):
    def __init__(self, axis):
        super().__init__()
        self.axis = axis

    def forward(self, tuple_of_x):
        return torch.cat(tuple_of_x, dim=self.axis)

    def string(self):
        return "ConcatinateLayer: " + str(self.axis)
    
def make_layer_impl(params_dict): #todo rename size -> params
    type_name = params_dict['type']
    if type_name == 'Conv1d': # nn layer
        in_depth = params_dict['in depth']
        out_depth = params_dict['out depth']
        kernel = params_dict['kernel']
        stride = params_dict['stride']
        padding = params_dict['padding']
        dilitation = params_dict['dilitation']
        return nn.Conv1d(in_depth, out_depth, kernel, stride, padding, dilitation) # in depth, out depth, kernel, stride, add padding
    if type_name == 'Conv2d': # nn layer
        in_depth = params_dict['in depth']
        out_depth = params_dict['out depth']
        kernel = params_dict['kernel']
        stride = params_dict['stride']
        padding = params_dict['padding']
        dilitation = params_dict['dilitation']
        return nn.Conv2d(in_depth, out_depth, kernel, stride, padding, dilitation) # in depth, out depth, kernel, stride, add padding
    
    elif type_name == 'Linear':
        return nn.Linear(params_dict['input size'], params_dict['output size']) # in, out
    
    elif type_name == 'Pool':
        kernel = params_dict['kernel']
        stride = params_dict['stride']
        padding = params_dict['padding']
        dilitation = params_dict['dilitation']
        return nn.MaxPool1d(kernel, stride, padding, dilitation)
    elif type_name == 'Pool2d':
        kernel = params_dict['kernel']
        stride = params_dict['stride']
        padding = params_dict['padding']
        dilitation = params_dict['dilitation']
        return nn.MaxPool2d(kernel, stride, padding, dilitation)
    
    elif type_name == 'Activation_function':  # activation func
        func_name = params_dict['func']
        if func_name == 'Sigmoid':  # activation func
            return nn.Sigmoid()
        elif func_name == 'Tanh':
            return nn.Tanh()
        elif func_name == 'Relu':
            return nn.ReLU()
        elif func_name == 'LeakyRelu':
            negative_slope = 0.01 #todo
            return nn.LeakyReLU(negative_slope)
        elif func_name == 'Selu':
            return nn.SELU()
        elif func_name == 'Softmax':
            return nn.Softmax()
        else:
            return None
    
    elif type_name == 'Normalization':   # normalization
        if ('BatchNorm1D' == params_dict['normalization_type']):
            return nn.BatchNorm1d(params_dict['strange param'])
        elif ('LayerNorm' == params_dict['normalization_type']):
            return nn.LayerNorm(params_dict['strange param'])
        else:
            return None
    
    elif ('Embedding' == type_name): # embedding
        vocab_size = params_dict['vocab size']
        embedding_size = params_dict['embedding size']
        padding_idx = 0#params_dict['padding']
        return nn.Embedding(vocab_size + 1, embedding_size, padding_idx)

    elif ('Reshape' == type_name): # reshape
        return ReshapeLayer(params_dict['size'])
    
    elif ('Concatinate' == type_name): # concatinate
        return ConcatinateLayer(params_dict['axis'])
    
    elif 'Dropout' == type_name:  # dropout
        if ('Dropout' == params_dict['dropout_type']):
            return nn.Dropout(params_dict['p'])
        elif ('Dropout1d' == params_dict['dropout_type']):
            return nn.Dropout(params_dict['p'])
        elif ('AlphaDropout' == params_dict['dropout_type']):
            return nn.AlphaDropout(params_dict['p'])
        elif ('FeatureAlphaDropout' == params_dict['dropout_type']):
            return nn.FeatureAlphaDropout(params_dict['p'])
        else:
            return None
        
    elif ('Flatten' == type_name): # flatten
        return nn.Flatten(params_dict['from'], params_dict['to'])
    
    elif type_name == 'Recurrent':   # recurrent
        if ('RNN' == params_dict['recurrent_type']):
            return nn.RNN(input_size = params_dict['input_size'],
                         hidden_size = params_dict['hidden_size'],
                         num_layers = params_dict['num_layers'],
                         bias = params_dict['bias'],
                         bidirectional = params_dict['bidirectional'])
        elif ('LSTM' == params_dict['recurrent_type']):
            return nn.LSTM(input_size = params_dict['input_size'],
                         hidden_size = params_dict['hidden_size'],
                         num_layers = params_dict['num_layers'],
                         bias = params_dict['bias'],
                         bidirectional = params_dict['bidirectional'])
        elif ('GRU' == params_dict['recurrent_type']):
            return nn.GRU(input_size = params_dict['input_size'],
                         hidden_size = params_dict['hidden_size'],
                         num_layers = params_dict['num_layers'],
                         bias = params_dict['bias'],
                         bidirectional = params_dict['bidirectional'])
        else:
            return None
    
    else:
        return None
    
    
def make_initializer(name):
    if ('Xavier' == name):
        return torch.nn.init.xavier_uniform_
    elif ('Uniform' == name):
        return torch.nn.init.uniform_
    elif ('Normal' == name):
        return torch.nn.init.normal_
    elif ('Dirac' == name):
        return torch.nn.init.dirac_
    elif ('KaimingNormal' == name):
        return kaiming_normal_
    else:
        return None
    
def make_layer(layer_params_dict):

    layer_impl = make_layer_impl(layer_params_dict)
    
    weight_init = make_initializer(layer_params_dict.get('initializer'))
    if (not weight_init is None):
        weight_init(layer_impl.weight)
    
    return layer_impl

class Net(nn.Module):
    def __init__(self, layers, data_size):
        super(Net, self).__init__()
        self.data_size = data_size
        self.layers = layers
        
    def forward(self, x):
        
        for layer in self.layers:
            x = layer(x)
            if type(x) is tuple:
                x = x[0]
            
        return x
        
def make_net(net_params_dict):
    input_data_size = net_params_dict.get('data_size')
    
    layers = nn.Sequential()
    for layer_params_dict in net_params_dict['layers'] :
        layer = make_layer(layer_params_dict) 
        
        input_data_size = check_size(layer_params_dict['type'], layer_params_dict, input_data_size)
        print('layer out: ', input_data_size)
        
        layers.append(layer)
        
    return Net(layers, net_params_dict.get('data_size'))

def make_net_from_xml(path_to_xml):
    net_params = MLDataLib.make_net_params_from_xml(path_to_xml)
    return make_net(net_params)




