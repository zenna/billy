from setup import *

num_trials = 5

#Plan with our model
def sample_sparsely(model):
    dd

def build_inverse_model(current_model, tilt_hist, motor_hist):
    
# Initialise model
initial_model = ....
model = initial_model

for trial in range(num_trials):
    #Sparse sampling
    speed = sample_sparsely(model)

    #Build model
    model = build_inverse_model(model, tilt_hist, motor_hist)

# min_tilt = 10000
# max_tilt = -10000

# t_old = Tilt()

# while(1):
#     try:
#         t = Tilt()
#         t_diff = t - t_old
#         t_old = t

#         if t < min_tilt:
#             min_tilt = t
    
#         #print "min", min_tilt
    
#         if t > max_tilt:
#             max_tilt = t
    
#         #print "max", max_tilcatcht
#         print(t_diff)

#         if (t_diff > 0):
#             Rfor(t_diff*256/1024)
#             Lfor(t_diff*256/1024)
#         else:
#             Rback(-t_diff*256/1024)
#             Lback(-t_diff*256/1024)

#     except:
#         pass
