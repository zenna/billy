from setup import *

num_trials = 5

<<<<<<< HEAD
#Plan with our model
def sample_sparsely(model):
    dd
=======
while(1):
    try:
        t = Tilt()
        t_diff = t - t_old
        t_old = t
>>>>>>> b430b2c2eb82278f9667c10f383fb36938067cb5

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
    
<<<<<<< HEAD
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
=======
        #print "max", max_tilcatcht

        speed = 80
        print(t_diff)
        scalar = 10
        if (t_diff > 0):
            Rfor(t_diff*scalar*256/1024)
            Lfor(t_diff*scalar*256/1024)
	    print(t_diff*scalar*256/1024)
        else:
            Rback(-t_diff*scalar*256/1024)
            Lback(-t_diff*scalar*256/1024)
	    print(t_diff*scalar*256/1024)
>>>>>>> b430b2c2eb82278f9667c10f383fb36938067cb5

#     except:
#         pass
