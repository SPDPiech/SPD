from ortools.linear_solver import pywraplp
from pathlib import Path
from ortools.sat.python import cp_model
class RPQ():
    def __init__(self,r,p,q):
        self.R=r
        self.P=p
        self.Q=q
def CP(jobs,instanceName):
    variablesMaxValue=0
    for i in range(len(jobs)):
        variablesMaxValue+=(jobs[i].R+jobs[i].P+jobs[i].Q)
    model=cp_model.CpModel()
#variables:
    alfasMatrix={}
    for i in range(len(jobs)):
        for j in range(len(jobs)):
            alfasMatrix[i,j]=model.NewIntVar(0,1,"alfa"+str(i)+"_"+str(j))
    starts=[]
    for i in range(len(jobs)):
        starts.append(model.NewIntVar(0,variablesMaxValue,"starts"+str(i)))
    cmax=model.NewIntVar(0,variablesMaxValue,"cmax")
#constraints:
    for i in range(len(jobs)):
        model.Add(starts[i]>=jobs[i].R)
        model.Add(cmax>=starts[i]+jobs[i].P+jobs[i].Q)
    for i in range(len(jobs)):
        for j in range(i+1,len(jobs)):
            model.Add(starts[i]+jobs[i].P<=starts[j]
                +alfasMatrix[i,j]*variablesMaxValue)
            model.Add(starts[j]+jobs[j].P<=starts[i]
                +alfasMatrix[j,i]*variablesMaxValue)
            model.Add(alfasMatrix[i,j]+alfasMatrix[j,i]==1)
#solver:
    model.Minimize(cmax)
    solver=cp_model.CpSolver()
    status=solver.Solve(model)
    if(status is not cp_model.OPTIMAL):
        print("Notoptimal!")
    print(instanceName,"Cmax:",solver.ObjectiveValue())
    pi=[]
    for i in range(len(starts)):
        pi.append((i,solver.Value(starts[i])))
    pi.sort(key=lambda x:x[1])
    print(pi)

def GetRPQsFromFile(pathToFile):
    
    fullTextFromFile=Path(pathToFile).read_text()
    words=fullTextFromFile.replace("\n"," ").split(" ")
    words_cleaned=list(filter(None,words))
    numbers=list(map(int,words_cleaned))
    numberOfJobs=numbers[0]
    numbers.pop(0)
    numbers.pop(0)
    jobs=[]
    for i in range(numberOfJobs):
        jobs.append(RPQ(numbers[0],numbers[1],numbers[2]))
        numbers.pop(0)
        numbers.pop(0)
        numbers.pop(0)
    return jobs

if __name__=='__main__':
    
    file_paths=["in50.txt"]
    for i in range(len(file_paths)):
        jobs=GetRPQsFromFile(file_paths[i])
        CP(jobs,file_paths[i])
