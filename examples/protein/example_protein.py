import mdtraj as md
import pyArborX as aX

import os
import signal


def convertFrame(basedata, frameNo: int,indices, points ):

    counter = 0
    for coordinates in basedata.xyz[frameNo,indices]:
        points[counter][0] = coordinates[0]
        points[counter][1] = coordinates[1]
        points[counter][2] = coordinates[2]
        counter=counter+1


def run():
    #  aX.ScopeGuard(len(sys.argv),sys.argv)
    #  k.set_default_space(pk.OpenMP)
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

    execution_space=aX.ExecutionSpace()
    #  for frame in range(t.n_frames):
    for frame in range(1):

        queryPoints_device = aX.PointView('queryPoints_device',qi.size)
        databasePoints_device = aX.PointView('databasePoints_device',bi.size)

        queryPoints_host = queryPoints_device.create_mirror_view()
        databasePoints_host = databasePoints_device.create_mirror_view()

        convertFrame(t,frame,qi,queryPoints_host)
        convertFrame(t,frame,bi,databasePoints_host)

        queryPoints_device.deep_copy(queryPoints_host)
        databasePoints_device.deep_copy(databasePoints_host)

        withinQueries_device = aX.generateWithinQueries_device(execution_space,queryPoints_device,qi.size,radius)

        bvh = aX.BVH(execution_space,databasePoints_device)

        indices_device = aX.intView1D('indices_device',0)
        offsets_device = aX.intView1D('offsets_device',0)

        bvh.query(execution_space,withinQueries_device,indices_device,offsets_device)

        indices_host = indices_device.create_mirror_view()
        offsets_host = offsets_device.create_mirror_view()




if __name__ == '__main__':
    aX.initialize()
    aX.printConfig()
    run()
    aX.finalize()
