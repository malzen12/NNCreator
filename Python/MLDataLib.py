#!/usr/bin/env python
# coding: utf-8

import pandas as pd
import numpy as np

from sklearn.model_selection import train_test_split

import torch
from torch.utils.data import TensorDataset, DataLoader

from xml.etree import ElementTree

def read_data1(path = 'Data/Data1/', cnt = -1):
    X_train = pd.read_csv(path + 'X_train_Final.csv')
    y_train = pd.read_csv(path + 'y_train_Final.csv', names=['class']).replace(-1, 0)
    X_test = pd.read_csv(path + 'X_test_Final.csv')
    y_test = pd.read_csv(path + 'y_test_Final.csv', names=['class']).replace(-1, 0)
    
    return X_train, y_train, X_test, y_test

def read_data2(file, cnt = -1):
    Data = pd.read_csv(file, delimiter="\t", header=1, names=["id", "sentence", "type"])
    
    if (cnt < 0):
        cnt = len(Data.index)
    
    Data = Data.head(cnt)
    
    Data["sentence"] = Data["sentence"].map( lambda sentence: [float(i) for i in sentence.split(",")])
    
    return Data[["sentence", "type"]]

def read_w2v(cnt = -1):
    return read_data2('Data/Data2/w2v.csv', cnt)
    
def read_pr_w2v(cnt = -1):
    return read_data2('Data/Data2/pr_w2v.csv', cnt)
    
def read_fasttext(cnt = -1):
    return read_data2('Data/Data2/fasttext.csv', cnt)
    
def read_pr_fasttext(cnt = -1):
    return read_data2('Data/Data2/pr_fasttext.csv', cnt)

def read_data3(file = 'Data/Data3/sentence.csv', cnt = -1):
    Data = pd.read_csv(file, delimiter="\t", header=1, names=["id", "sentence", "type"])
    
    if (cnt < 0):
        cnt = len(Data.index)
    
    Data = Data.head(cnt)
    
    Data["sentence"] = Data["sentence"].map( lambda sentence: [int(i) for i in sentence[1:-1].split(",")])
    
    return Data[["sentence", "type"]]

def make_test_set(x_train, y_train, x_test, y_test, batch_size=1):
    inputs_train = torch.from_numpy(np.array(x_train.tolist()))
    targets_train = torch.from_numpy(y_train.values)

    inputs_test = torch.from_numpy(np.array(x_test.values.tolist()))
    targets_test = torch.from_numpy(y_test.values)

    train = TensorDataset(inputs_train, targets_train)
    test = TensorDataset(inputs_test, targets_test)

    trainset = DataLoader(train, batch_size=batch_size, shuffle=True, drop_last=True, pin_memory=True)
    testset = DataLoader(test, batch_size=batch_size, shuffle=False, drop_last=True, pin_memory=True)
    
    return trainset, testset

def data_to_testset(Data, batch_size=1):
    x_train, x_test, y_train, y_test = train_test_split(Data['sentence'], Data['type'], test_size=0.25, random_state=42)
    return make_test_set(x_train, y_train, x_test, y_test, batch_size)

def read_array_params(xml_node, param_name):
    return [ int(e.text) for e in xml_node.find(param_name).iter('v')]

def read_param(xml_node, param_name):
    if (not xml_node.find(param_name) is None):
            return xml_node.find(param_name).text
    else:
        raise RuntimeError('param {} not found'.format(param_name))

def read_linear(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['input size'] = int(read_param(xml_node, 'input_size'))
    params['output size'] = int(read_param(xml_node, 'output_size'))
    params['initializer'] = read_param(xml_node, 'Initializer_function')
    return params;

def read_conv1d(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['in depth'] = int(read_param(xml_node, 'in_depth'))
    params['out depth'] = int(read_param(xml_node, 'out_depth'))
    params['kernel'] = int(read_param(xml_node, 'kernel_size'))
    params['stride'] = int(read_param(xml_node, 'stride'))
    params['padding'] = int(read_param(xml_node, 'padding'))
    params['dilitation'] = int(read_param(xml_node, 'dilitation'))
    params['initializer'] = read_param(xml_node, 'Initializer_function')
    return params;

def read_conv2d(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['in depth'] = int(read_param(xml_node, 'in_depth'))
    params['out depth'] = int(read_param(xml_node, 'out_depth'))
    params['kernel'] = read_array_params(xml_node, 'kernel_size')[:2]
    params['stride'] = read_array_params(xml_node, 'stride')[:2]
    params['padding'] = read_array_params(xml_node, 'padding')[:2]
    params['dilitation'] = read_array_params(xml_node, 'dilitation')[:2]
    params['initializer'] = read_param(xml_node, 'Initializer_function')
    return params;

def read_pool(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['kernel'] = int(read_param(xml_node, 'kernel_size'))
    params['stride'] = int(read_param(xml_node, 'stride'))
    params['padding'] = int(read_param(xml_node, 'padding'))
    params['dilitation'] = int(read_param(xml_node, 'dilitation'))
    return params;   

def read_pool2d(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['kernel'] = read_array_params(xml_node, 'kernel_size')[:2]
    params['stride'] = read_array_params(xml_node, 'stride')[:2]
    params['padding'] = read_array_params(xml_node, 'padding')[:2]
    params['dilitation'] = read_array_params(xml_node, 'dilitation')[:2]
    return params;  

def read_embedding(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['vocab size'] = int(read_param(xml_node, 'vocab_size'))
    params['embedding size'] = int(read_param(xml_node, 'embedding_size'))
    
    return params; 

def read_reshape(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['size'] = read_array_params(xml_node, 'size')
    return params

def read_normalization(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['normalization_type'] = read_param(xml_node, 'Normalization')
    params['strange param'] = int(read_param(xml_node, 'strange_param'))
    
    return params

def read_activation(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['func'] = read_param(xml_node, 'Activation_function')
    return params 

def read_concatinate(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['axis'] = int(read_param(xml_node, 'axis'))
    return params

def read_dropout(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['dropout_type'] = read_param(xml_node, 'Dropout')
    params['p'] = float(read_param(xml_node, 'p'))
    
    return params

def read_flatten(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    axis_to_flat = read_array_params(xml_node, 'axis_to_flat')
    params['from'] = axis_to_flat[0]
    params['to'] = axis_to_flat[1]
    
    return params

def read_recurrent(xml_node):
    params = {}
    params['id'] = int(read_param(xml_node, 'id'))
    params['type'] = read_param(xml_node, 'type')
    params['forwards'] = read_array_params(xml_node, 'forwards')
    
    params['recurrent_type'] = read_param(xml_node, 'Recurrent')
    params['input_size'] = int(read_param(xml_node, 'input_size'))
    params['hidden_size'] = int(read_param(xml_node, 'hidden_size'))
    params['num_layers'] = int(read_param(xml_node, 'num_layers'))
    params['bias'] = bool(read_param(xml_node, 'bias'))
    params['bidirectional'] = bool(int(read_param(xml_node, 'bidirectional')))
    
    return params

def read_layer(xml_node):
    type_name = read_param(xml_node, 'type')
    if ('Linear' == type_name):
        return read_linear(xml_node)
    elif ('Conv1d' == type_name):
        return read_conv1d(xml_node)
    elif ('Conv2d' == type_name):
        return read_conv2d(xml_node)
    elif ('Pool' == type_name):
        return read_pool(xml_node)
    elif ('Pool2d' == type_name):
        return read_pool2d(xml_node)
    elif ('Embedding' == type_name):
        return read_embedding(xml_node)
    elif ('Reshape' == type_name):
        return read_reshape(xml_node)
    elif ('Normalization' == type_name):
        return read_normalization(xml_node)
    elif ('Activation_function' == type_name):
        return read_activation(xml_node)
    elif ('Concatinate' == type_name):
        return read_concatinate(xml_node)
    elif ('Dropout' == type_name):
        return read_dropout(xml_node)
    elif ('Flatten' == type_name):
        return read_flatten(xml_node)
    elif ('Recurrent' == type_name):
        return read_recurrent(xml_node)
    else:
        raise RuntimeError('invalid type {} passed'.format(type_name))

def make_net_params_from_xml(path_to_xml):
    net_params = {}
    
    tree = ElementTree.parse(path_to_xml)
    
    net_params['data_size'] = read_array_params(tree, 'data_size')
    
    layers = []
    for elem in tree.find('layers').iter('l'):
        layer = read_layer(elem)
        layers.append(layer)
    
    net_params['layers'] = layers
    return net_params

def make_train_params_from_xml(path_to_xml):
    train_params = {}
    
    tree = ElementTree.parse(path_to_xml)
    
    train_params['epoch_cnt'] = int(read_param(tree, 'Epoch_cnt'))
    train_params['scheduler'] = read_param(tree, 'Scheduler')
    train_params['optimizer'] = read_param(tree, 'Optimizer')
    train_params['optimizer_speed'] = float(read_param(tree, 'Optimizer_speed'))
    train_params['weight_decay'] = float(read_param(tree, 'weight_decay'))
    
    return train_params




