#include <pqp_ros_server.h>
using namespace std;


/* functions */
PQP_Model readmodel(string name);
bool pqpSrvFunc(pqp_server::pqpRequest::Request &req,
		pqp_server::pqpRequest::Response &res);
/* Global PQP models */
PQP_Model m1 = readmodel("room");
PQP_Model m2 = readmodel("piano");

/*
 * Read trigles from txt file
 * Build PQP model using trigles
 * Return PQP model
 * */
PQP_Model readmodel(string name)
{
    PQP_Model m;
    m.BeginModel();
    // read from model file
    ifstream infile;
    string file_name = "mesh/" + name + ".txt";
    char file_name_char[file_name.size() + 1];
    strcpy(file_name_char, file_name.c_str());
    infile.open(file_name_char);
    if(!infile)
    {
        cout<<"Wrong model name for collision checker"<<endl;
        exit(EXIT_FAILURE);
    }
    int tri_num;
    infile >> tri_num;
    cout<<"number of triangles for "<<name<<": "<<tri_num<<endl;
    for(int num=0; num<tri_num; num++)
    {
        PQP_REAL p[3][3];
        for(int r=0; r<3; r++)
        {
            for(int c=0; c<3; c++)
            {
                double v;
                infile>>v;
                p[r][c] = v;
            }
        }
        m.AddTri(p[0], p[1], p[2], num);
    }
    infile.close();
    m.EndModel();
    cout<<"model "<<name<<" loaded..."<<endl;
    return m;
}


bool pqpSrvFunc(pqp_server::pqpRequest::Request &req,
		pqp_server::pqpRequest::Response &res)
{
	ROS_INFO("PQP request received...");
	PQP_REAL T1[3];
	PQP_REAL T2[3];
	PQP_REAL R1[3][3];
	PQP_REAL R2[3][3];
	for(int i=0; i<3; i++)
	{
		T1[i] = 0;
		T2[i] = req.T[i];
		for(int j=0; j<3; j++)
		{
			R1[i][j] = 0;
			if(i == j)
			{
				R1[i][j] = 1;
			}
			R2[i][j] = req.R[i*3+j];
		}
	}
	PQP_CollideResult cres;
	PQP_Collide(&cres, R1, T1, &m1, R2, T2, &m2);
	int colliding = cres.Colliding();
	if(colliding == 0)
	{
		ROS_INFO("No collision");
		res.result = false;
	}
	else
	{
		ROS_INFO("Collision");
		res.result = true;
	}
	return true;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "pqp_ros_server");
	ros::NodeHandle nh;
	ros::ServiceServer service = nh.advertiseService("pqp_server", pqpSrvFunc);
	ROS_INFO("PQP ROS server starts...");
	ros::spin();
	return EXIT_SUCCESS;
}
