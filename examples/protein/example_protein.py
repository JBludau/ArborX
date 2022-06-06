import mdtraj as md
import pyArborX as aX

import time
import os
import signal


def convertFrame(basedata, frameNo: int,indices, points ):

    for index in indices:
        points[index][0] = basedata.xyz[frameNo,index,0]
        points[index][1] = basedata.xyz[frameNo,index,1]
        points[index][2] = basedata.xyz[frameNo,index,2]

def writeRawResults(indices,offsets,filename):
    with open(filename,'w') as f:
        for i in range(indices.size()):
            f.write(f"{indices[i]} ")
        f.write("\n")
        for i in range(offsets.size()):
            f.write(f"{offsets[i]} ")
        f.write("\n")

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
    queryPoints_deviceFrames = []
    queryPoints_hostFrames = []
    databasePoints_deviceFrames = []
    databasePoints_hostFrames = []
    startReadIn = time.time()
    for frame in range(t.n_frames):
        queryPoints_deviceFrames.append(aX.PointView('queryPoints_device',qi.size))
        databasePoints_deviceFrames.append(aX.PointView('databasePoints_device',bi.size))

        queryPoints_hostFrames.append(queryPoints_deviceFrames[frame].create_mirror_view())
        databasePoints_hostFrames.append(databasePoints_deviceFrames[frame].create_mirror_view())

        convertFrame(t,frame,qi,queryPoints_hostFrames[frame])
        convertFrame(t,frame,bi,databasePoints_hostFrames[frame])

        queryPoints_deviceFrames[frame].deep_copy(queryPoints_hostFrames[frame])
        databasePoints_deviceFrames[frame].deep_copy(databasePoints_hostFrames[frame])

        print(f"read frame {frame}")

    endReadIn=time.time()
    for frame in range(t.n_frames):
    #  for frame in range(1):

        print(f"runing setup for frame {frame}")

        withinQueries_device = aX.generateWithinQueries_device(execution_space,queryPoints_deviceFrames[frame],queryPoints_deviceFrames[frame].size(),radius)

        bvh = aX.BVH(execution_space,databasePoints_deviceFrames[frame])

        indices_device = aX.intView1D('indices_device',0)
        offsets_device = aX.intView1D('offsets_device',0)

        print(f"runing query for frame {frame}")
        startQueryTime = time.time()
        bvh.query(execution_space,withinQueries_device,indices_device,offsets_device)
        endQueryTime = time.time()

        print(f"copying data back to host for frame {frame}")

        indices_host = indices_device.create_mirror_view()
        offsets_host = offsets_device.create_mirror_view()

        indices_host.deep_copy(indices_device)
        offsets_host.deep_copy(offsets_device)

        print(f"writing output for frame {frame}")

        #  writeRawResults(indices_host,offsets_host,"arborx_raw.txt")

        print(f"finished frame {frame}")
    endQuery = time.time()

    print(f"readin time {endReadIn-startReadIn}")
    print(f"query loop time {endQuery-endReadIn}")
    print(f"query time single run {endQueryTime-startQueryTime}")

if __name__ == '__main__':
    aX.initialize()
    aX.printConfig()
    run()
    aX.finalize()
