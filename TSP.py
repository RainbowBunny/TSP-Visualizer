import gurobipy as gp
from gurobipy import GRB
import numpy as np
import sys

from math import sqrt

# x[i][j]: place vertex i in position j

class TSP:
    def __init__(self, points):
        self.points = points
        self.distance = [[0 for i in range(len(points))] for j in range(len(points))]
        self.model = gp.Model("TSP")
        self.model.setParam('OutputFlag', False)
        for i in range(len(points)):
            for j in range(len(points)):
                self.distance[i][j] = sqrt((points[i][0] - points[j][0]) ** 2 + (points[i][1] - points[j][1]) ** 2)

    def createVariables(self):
        self.x = [[self.model.addVar(vtype = GRB.BINARY, name = f'x_{i}_{j}') for i in range(len(self.points))] for j in range(len(self.points))]
    
    def oneOccurence(self):
        for i in range(len(self.points)):
            numberOfOccurences = [self.x[i][j] for j in range(len(self.points))]
            self.model.addConstr(sum(numberOfOccurences) == 1)
    
    def uniquePosition(self):
        for j in range(len(self.points)):
            positionList = [self.x[i][j] for i in range(len(self.points))]
            self.model.addConstr(sum(positionList) == 1)
    
    def setObjective(self):
        costList = []
        for i in range(len(self.points)):
            for j in range(len(self.points)):
                for k in range(len(self.points)):
                    costList.append(self.x[i][k] * self.x[j][(k + 1) % len(self.points)] * self.distance[i][j])
        self.model.setObjective(sum(costList), GRB.MINIMIZE)

    def getSolution(self):
        self.answer = [0 for i in range(len(self.points))]
        for i in range(len(self.points)):
            for j in range(len(self.points)):
                if self.x[i][j].x > 0.5:
                    self.answer[j] = i

    def solve(self):
        self.createVariables()
        self.oneOccurence()
        self.setObjective()
        self.uniquePosition()
        self.model.optimize()
        self.getSolution()

if __name__ == '__main__':
    PointList = []
    with open("data/input.txt", "r") as f:
        n = int(f.readline().strip())
        for i in range(n):
            x, y = list(map(int, f.readline().strip().split()))
            PointList.append([x, y])
    solver = TSP(PointList)
    solver.solve()
    outfile = open("data/output.txt", "w")
    print(solver.answer, file = outfile)