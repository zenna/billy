min_tilt = 10000
max_tilt = -10000

t_old = Tilt()

while(1):
    try:
        t = Tilt()
        t_diff = t t_old
        t_old = t

        if t < min_tilt:
            min_tilt = t
    
        #print "min", min_tilt
    
        if t > max_tilt:
            max_tilt = t
    
        #print "max", max_tilcatcht

        speed = 80
        print(t_diff)

        if (t_diff > 0):
            Rfor(t_diff*256/1024)
            Lfor(t_diff*256/1024)
        else:
            Rback(-t_diff*256/1024)
            Lback(-t_diff*256/1024)

    except:
        pass
