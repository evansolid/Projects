import pygame
from pygame.locals import *
import random
import math

class Life:
    def __init__(self,width,height,color):
        pygame.init()
        self.width = width
        self.height = height
        self.color = color
        self.screen = pygame.display.set_mode((self.width,self.height))

        self.screen.fill(color)

        pygame.display.update()
        self.running = True

        # initialize clock
        self.clock = pygame.time.Clock()

        # define colors 
        self.red = (255,0,0)
        self.green = (0,255,0)
        self.blue = (0,0,255)
        self.yellow = (255,255,0)

    # function to create a particle
    def createParticle(self,x,y,color,vx,vy):
        newParticle = {"x":x,"y":y,"color":color,"vx":vx,"vy":vy}
        return newParticle
    
    # function to draw a particle
    def drawParticle(self,x,y,color):
        pygame.draw.rect(self.screen,color,(x,y,5,5))

    # function to initialize many points of a color
    def manyParticles(self,number,color):
        group = []
        for i in range(number):
            x = random.randint(1,self.width-1)
            y = random.randint(1,self.height-1)
            vx = 0
            vy = 0

            group.append(self.createParticle(x,y,color,vx,vy))
        return group
    
    # function to draw many points
    def drawParticles(self,group):
        for i in group:
            self.drawParticle(i["x"],i["y"],i["color"])

    # function to find gravity between particles
    def gravity(self,p1,p2,g):
        cutoff = 500

        if g != 0:
            for i in range(len(p1)):
                fx = 0
                fy = 0

                for j in range(len(p2)):
                    a = p1[i]
                    b = p2[j]

                    dx = b["x"] - a["x"]
                    dy = b["y"] - a["y"]
                    d = dx*dx+dy*dy

                    # Limit where the gravity is checked
                    if d < 1 or d > cutoff:
                        continue

                    F = g/d
                    fx += F*dx
                    fy += F*dy

                a["vx"] = (a["vx"] + fx)*0.5
                a["vy"] = (a["vy"] + fy)*0.5
                a["x"] += a["vx"]
                a["y"] += a["vy"]

                # reverse the particles if they hit a wall

                if (a["x"] <= 0):
                    a["x"] = 1
                    a["vx"] *= -1
                elif (a["x"] >= self.width):
                    a["x"] = self.width-1
                    a["vx"] *= -1            
                if (a["y"] <= 0):
                    a["y"] = 1
                    a["vy"] *= -1            
                elif (a["y"] >= self.height):
                    a["y"] = self.height-1
                    a["vy"] *= -1

    # function to apply the gravity function between each color of particle
    def gravities(self,colors,gravities):
        for i in range(len(colors)):
            for j in range(len(colors)):
                self.gravity(colors[i],colors[j],gravities[i][j])

    # return a list of four floats between [x,y]
    def listFour(self,x,y):
        return [random.uniform(x,y) for i in range(4)]
    
    # main loop
    def gameLoop(self):
        # define particles of each color
        self.redParticles    = self.manyParticles(100,self.red)
        self.greenParticles  = self.manyParticles(100,self.green)
        self.blueParticles   = self.manyParticles(100,self.blue)
        self.yellowParticles = self.manyParticles(100,self.yellow)

        colors = [self.redParticles,self.greenParticles,self.blueParticles,self.yellowParticles]

        # define the gravity rules between each color
        # strength between self and {red,green,blue,yellow}
        self.redGravity    = self.listFour(-1.0,1.0)
        self.greenGravity  = self.listFour(-1.0,1.0)
        self.blueGravity   = self.listFour(-1.0,1.0)
        self.yellowGravity = self.listFour(-1.0,1.0)

        gravities = [self.redGravity,self.greenGravity,self.blueGravity,self.yellowGravity]

        # While the game is running:
        while self.running:
            for event in pygame.event.get():
                if event.type == QUIT:
                    self.running = False
                elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                    self.redGravity    = self.listFour(-1.0,1.0)*5
                    self.greenGravity  = self.listFour(-1.0,1.0)*5
                    self.blueGravity   = self.listFour(-1.0,1.0)*5
                    self.yellowGravity = self.listFour(-1.0,1.0)*5
                    gravities = [self.redGravity,self.greenGravity,self.blueGravity,self.yellowGravity]
            # draw the particles
            self.screen.fill(self.color)
            self.drawParticles(self.redParticles)
            self.drawParticles(self.greenParticles)
            self.drawParticles(self.blueParticles)
            self.drawParticles(self.yellowParticles)

            # update the display
            pygame.display.update()

            # calculate gravity between points
            self.gravities(colors,gravities)

            # clock tick
            self.clock.tick(120)
        pygame.quit()

# main function

particleLife = Life(500,300,(255,255,255))
particleLife.gameLoop()

