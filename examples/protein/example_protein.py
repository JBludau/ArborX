import mdtraj as md
import kokkos
import pykokkos as pk
import pyArborX as aX


def convertFrame(basedata, frameNo: int,indices, viewName: str ):

    points = aX.allocWithoutInitialize(viewName,indices.size)
    counter = 0
    for coordinates in basedata.xyz[frameNo,indices]:
        points[counter][0] = coordinates[0]
        points[counter][1] = coordinates[1]
        points[counter][2] = coordinates[2]
        counter=counter+1

    return points
    
if __name__ == '__main__':
    pk.set_default_space(pk.OpenMP)
    #  pk.enable_uvm()
    ## READIN

    t = md.load('./data/trajectory.xtc', top='./data/coordinates.gro')
    # The trajectory has 1251 frames
    print("n total frames", t.n_frames)
    # the trajectory is quite large, so for testing we can just use the first 10 frames:
    t = t[:10]
    print("reduces n frames", t.n_frames)

    # t is the trajectory object, and t.xyz stores the xyz coordinates.
    # The shape is: (time, points, coordinates)
    print ("t_shape", t.xyz.shape)
    print (t.xyz[0, :2, :]) # xyz coordinates of the first two points of the first frame

    # We get the indices for the query and database points
    qi = t.topology.select('protein') # query points for which we need to find nearest neighbors
    bi = t.topology.select('not protein') # database points
    
    print (qi.size)

    # The radius we will use to search for nearest neighbors
    radius = 0.7
 

    ## TRANSFORM TO KOKKOS/ARBORX OBJECTS
    for frame in range(t.n_frames):
        queryPoints = convertFrame(t,frame,qi,'queryPoints')
        databasePoints = convertFrame(t,frame,bi,'databasePoints')

    ## FINALIZE KOKKOS 
    pk.finalize()
