'''
Task: We will analyze these data to determine what (if any) activity is evoked (so
called Event-related potential, ERP) following two different types of stimuli presented to
a human subject. Specifically,
5
1. Visualize the response, i.e., ERP of the EEG, in the two conditions, A and B.
2. Find the brain activity frequency in the data of condition A (see below for condition
A and B).
'''

import scipy.io
mat = scipy.io.loadmat('02_EEG-1.mat')

