# pip install dlib
# pip install face_recognition
import cv2
import face_recognition as face

# Cargamos la cara la cual queremos RECONOCER
frame = cv2.imread("cara.png")

framergb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

# Localizamos la cara de la imagen
locs = face.face_locations(framergb)

if locs is not None:
    for i in range(len(locs)):
        t,r,b,l = locs[i] # top, right, bottom ,left
        # Diccionario de facciones de la cara
        lands = face.face_landmarks(framergb[t:b, l:r]) 
        cv2.rectangle(frame, (l,t), (r,b), (255,0,0), 2)

cod_micara = face.face_encodings(framergb,locs,model='small')[0]

#ahora la que tengo la codificacion de mi cara, en la camara uso lo siguiente
print("abrimos cam\n")
cap = cv2.VideoCapture(0)
final = False
while not final:
    #Leemos el frame en formato BGR
    ret, framebgr = cap.read()
    if ret:
        #mini = cv2.resize(framergb, dsize=(300,300),fx=0.25, fy=0.25)
        #mini = framebgr
        locs = face.face_locations(framebgr, model='hog') # cnn o hog 
        cods = face.face_encodings(framebgr, locs, model='small') #podemos cambiar de larga a small para obtener mejor rendimeinto pero peor calidad
        if locs is not None:
            for i in range(len(locs)):
                if face.compare_faces(cod_micara, [cods[i]])[0]:
                    color = (0,255,0)
                else:
                    color = (0,0,255)
                t,r,b,l = locs[i] # top, right, bottom ,left
                dist = face.face_distance(cod_micara,[cods[i]])[0]
                
                cv2.rectangle(framebgr, (l,t), (r,b), color, 2)
                cad = "Error " + str('%.3f'%dist)
                img = cv2.cvtColor(framebgr, cv2.COLOR_BGR2RGB)
                cv2.putText(framebgr, cad, (l,b+20), cv2.FONT_HERSHEY_TRIPLEX, 1, color)
        cv2.imshow("Video", framebgr)
        if cv2.waitKey(1) == ord(' '):
            final = True
    else:
        final = True