#!/usr/bin/env python
# coding: utf-8

import pandas as pd
import numpy as np

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

import matplotlib.pyplot as plt

from sklearn.metrics import classification_report

from tqdm import tqdm
from IPython.display import clear_output

def make_optimizer(params, model):
    if ('SGD' == params['optimizer']):
        return optim.SGD(model.parameters(), lr=params['optimizer_speed'])
    
    elif ('Adam' == params['optimizer']):
        return optim.Adam(model.parameters(), lr=params['optimizer_speed'])
    
    elif ('AdamW' == params['optimizer']):
        return optim.AdamW(model.parameters(), lr=params['optimizer_speed'])
    
    else:
        return optim.SGD(model.parameters(), lr=params['optimizer_speed'])

def make_scheduler(name, optimizer, epoch_cnt):
    if ('Exponentioal' == name):
        return optim.lr_scheduler.ExponentialLR(optimizer, gamma=0.1)
    elif ('Linear' == name):
        return optim.lr_scheduler.LinearLR(optimizer)
    elif ('MultiStep' == name):
        return optim.lr_scheduler.MultiStepLR(optimizer, milestones=[int(epoch_cnt*0.3), int(epoch_cnt*0.6)])
    else:
        return None

def train_net(dataset, net, params):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    
    net.to(device)
    
    print(device)
    
    optimizer = make_optimizer(params, net)
    scheduler = make_scheduler(params.get('scheduler'), optimizer, params['epoch_cnt'])

    train_logs = []
    for epoch in tqdm(range(params['epoch_cnt'])):
        for X, y in dataset:
            
            net.zero_grad() # todo is it need?
            
            output = net(X.to(device))
            
            loss = F.cross_entropy(output, y.to(device).long()) # reshape to [batch size, class cnt] 
            loss.backward()
            
            optimizer.step()
            
            if (None != scheduler):
                scheduler.step()
            
        train_logs.append(loss.item())
        clear_output(wait=True)
    
    plt.plot(train_logs)
    plt.show()
    
    return train_logs

def test_net(net, dataset):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    net.to(device)
    
    ans = []
    origin = []
    
    with torch.no_grad(): # todo 
        for data in tqdm(dataset):
            X, y = data
            output = net(X.to(device))
            for idx, i in enumerate(output):
                ans.append(torch.argmax(i).item())
                
            origin += y.tolist()
            clear_output(wait=True)
    
    result_metric = classification_report(origin, ans, output_dict=True)
#     print(result_metric)
    return result_metric


def dict_to_str(d):
    return ''.join([ str(key) + ":" + str(d[key]) + '\n' for key in d.keys()])

def make_log_dataframe(net, train_params, result):
    return pd.DataFrame(data={'net':[str(net)], 
                              'train_params': [dict_to_str(train_params)], 
                              'result f1': [str(result['accuracy'])]})

def print_log_dataframe(log_dataframe):
    display(log_dataframe.style.set_properties(**{'white-space': 'pre', 'text-align' : 'left'}))



