#include "cxx_3d_seg.h"
#include <chrono>

using namespace std;
using namespace cv;
// for test
namespace test_helper {
class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count(); }
    void out(std::string message = ""){
        double t = elapsed();
        std::cout << message << "  elasped time:" << t << "s" << std::endl;
        reset();
    }
private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};
string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
template<typename T>
std::vector<T> unique(const cv::Mat& input, bool sort = true)
{
    std::vector<T> out;
    for (int y = 0; y < input.rows; ++y)
    {
        auto row_ptr = input.ptr<T>(y);
        for (int x = 0; x < input.cols; ++x)
        {
            T value = row_ptr[x];

            if ( std::find(out.begin(), out.end(), value) == out.end() )
                out.push_back(value);
        }
    }

    if (sort)
        std::sort(out.begin(), out.end());

    return out;
}
}

void dataset_test(){
    int train_size = 1000;
//    string prefix = "/home/meiqua/6DPose/public/datasets/doumanoglou/test/01/";
    string prefix = "/home/meiqua/6DPose/public/datasets/tejani/test/06/";
    for(int i=0;i<train_size;i++){
        auto i_str = to_string(i);
        for(int pad=4-i_str.size();pad>0;pad--){
            i_str = '0'+i_str;
        }
        Mat rgb = cv::imread(prefix+"rgb/"+i_str+".png");
        Mat depth = cv::imread(prefix+"depth/"+i_str+".png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

        test_helper::Timer timer;

        auto rgb_slimage = slimage::ConvertToSlimage(rgb);
        auto dep_slimage = slimage::ConvertToSlimage(depth);
        slimage::Image3ub img_color = slimage::anonymous_cast<unsigned char,3>(rgb_slimage);
        slimage::Image1ui16 img_depth = slimage::anonymous_cast<uint16_t,1>(dep_slimage);

        auto test_group = asp::DsapGrouping(img_color, img_depth);
        Mat idxs = slimage::ConvertToOpenCv(test_group);

        timer.out("grouping");

        std::vector<int> unik = test_helper::unique<int>(idxs, true);
        std::map<int, Vec3b> color_map;
        for(auto idx: unik){
            auto color = Vec3b(rand()%255, rand()%255, rand()%255);
            color_map[idx] = color;
        }

        Mat show = Mat(idxs.size(), CV_8UC3, Scalar(0));
        auto show_iter = show.begin<Vec3b>();
        for(auto idx_iter = idxs.begin<int>(); idx_iter<idxs.end<int>();idx_iter++, show_iter++){
            if(*idx_iter>0){
                auto color = color_map.find(*idx_iter)->second;
                *show_iter = color;
            }
        }

        imshow("show", show);
        imshow("rgb", rgb);
        waitKey(3000);
    }
}

void simple_test(){
    string prefix = "/home/meiqua/6DPose/cxx_3d_seg/test/2/";
    Mat rgb = cv::imread(prefix+"rgb/0001.png");
    Mat depth = cv::imread(prefix+"depth/0001.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

//    pyrDown(rgb, rgb);
//    pyrDown(depth, depth);

    test_helper::Timer timer;

    auto rgb_slimage = slimage::ConvertToSlimage(rgb);
    auto dep_slimage = slimage::ConvertToSlimage(depth);
    slimage::Image3ub img_color = slimage::anonymous_cast<unsigned char,3>(rgb_slimage);
    slimage::Image1ui16 img_depth = slimage::anonymous_cast<uint16_t,1>(dep_slimage);

    auto test_group = asp::DsapGrouping(img_color, img_depth);
    Mat idxs = slimage::ConvertToOpenCv(test_group);

    timer.out("grouping");

    std::vector<int> unik = test_helper::unique<int>(idxs, true);
    std::map<int, Vec3b> color_map;
    for(auto idx: unik){
        auto color = Vec3b(rand()%255, rand()%255, rand()%255);
        color_map[idx] = color;
    }

    Mat show = Mat(idxs.size(), CV_8UC3, Scalar(0));
    auto show_iter = show.begin<Vec3b>();
    for(auto idx_iter = idxs.begin<int>(); idx_iter<idxs.end<int>();idx_iter++, show_iter++){
        if(*idx_iter>0){
            auto color = color_map.find(*idx_iter)->second;
            *show_iter = color;
        }
    }
    imshow("show", show);
    imshow("rgb", rgb);
    waitKey(0);
//    Mat rgb_ = slimage::ConvertToOpenCv(rgb_slimage);
//    Mat depth_ = slimage::ConvertToOpenCv(dep_slimage);

}

void super4pcs_test(){
    string prefix = "/home/meiqua/6DPose/cxx_3d_seg/test/2/";
    Mat rgb = cv::imread(prefix+"rgb/0001.png");
    Mat depth = cv::imread(prefix+"depth/0001.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

    test_helper::Timer timer;

    auto rgb_slimage = slimage::ConvertToSlimage(rgb);
    auto dep_slimage = slimage::ConvertToSlimage(depth);
    slimage::Image3ub img_color = slimage::anonymous_cast<unsigned char,3>(rgb_slimage);
    slimage::Image1ui16 img_depth = slimage::anonymous_cast<uint16_t,1>(dep_slimage);

    auto test_group = asp::DsapGrouping(img_color, img_depth);
    Mat idxs = slimage::ConvertToOpenCv(test_group);

    timer.out("grouping");

    int test_which = 12;
    int test_count = 0;
    Mat show = Mat(idxs.size(), CV_8UC3, Scalar(0));
    auto show_iter = show.begin<Vec3b>();
    for(auto idx_iter = idxs.begin<int>(); idx_iter<idxs.end<int>();idx_iter++, show_iter++){
        if(*idx_iter==test_which){
            *show_iter = {0, 0, 255};
            test_count ++;
        }
    }
    std::cout << "test_count: " << test_count << std::endl;

    imshow("rgb", rgb);
    imshow("show", show);

    waitKey(0);

    timer.reset();
    Mat test_seg = idxs == test_which;

//    Mat test_seg = imread(prefix+"test_seg.png");
//    cvtColor(test_seg, test_seg, CV_BGR2GRAY);

    Mat test_dep;
    depth.copyTo(test_dep, test_seg);

    Mat sceneK = (Mat_<float>(3,3)
                  << 550.0, 0.0, 316.0, 0.0, 540.0, 244.0, 0.0, 0.0, 1.0);
    cv::Mat sceneCloud;
    cv::rgbd::depthTo3d(test_dep, sceneK, sceneCloud);

    std::vector<GlobalRegistration::Point3D> test_cloud;

    for(auto cloud_iter = sceneCloud.begin<cv::Vec3f>();
        cloud_iter!=sceneCloud.end<cv::Vec3f>(); cloud_iter++){
        if(cv::checkRange(*cloud_iter)){
            GlobalRegistration::Point3D p;
            p.x() = (*cloud_iter)[0]*1000;
            p.y() = (*cloud_iter)[1]*1000;
            p.z() = (*cloud_iter)[2]*1000;
            test_cloud.push_back(p);
        }
    }

    std::vector<GlobalRegistration::Point3D> model_v;
    std::vector<typename GlobalRegistration::Point3D::VectorType> model_n;
    {
        IOManager iom;
        std::vector<Eigen::Matrix2f> tex_coords;
        std::vector<tripple> tris;
        std::vector<std::string> mtls;
        iom.ReadObject((prefix+"model.ply").c_str(), model_v, tex_coords, model_n, tris, mtls);
    }

    Eigen::Matrix4f	transformation = Eigen::Matrix4f::Identity();
    float score = 0;
    {
        GlobalRegistration::Match4PCSOptions options;
        options.sample_size = 200;
        options.max_time_seconds = 1;
        constexpr GlobalRegistration::Utils::LogLevel loglvl = GlobalRegistration::Utils::Verbose;
        GlobalRegistration::Utils::Logger logger(loglvl);
        GlobalRegistration::MatchSuper4PCS matcher(options, logger);
        score = matcher.ComputeTransformation(model_v, &test_cloud, transformation);
    }
    std::cout << "final LCP: " << score << std::endl;
    cout << transformation.inverse() << endl;
    timer.out("super4pcs");

    int model_icp_size = 10000;
    if(model_icp_size > model_v.size()) model_icp_size = model_v.size();
    int model_icp_step = model_v.size()/model_icp_size;
    std::vector<cv::Vec3f> model_v_eigen(model_icp_size);
    for(int i=0; i<model_icp_size; i+=1){
        model_v_eigen[i](0) = model_v[i*model_icp_step].x();
        model_v_eigen[i](1) = model_v[i*model_icp_step].y();
        model_v_eigen[i](2) = model_v[i*model_icp_step].z();
    }

    int cloud_icp_size = 1000;
    if(cloud_icp_size > test_cloud.size()) cloud_icp_size = test_cloud.size();
    int cloud_icp_step = test_cloud.size()/cloud_icp_size;
    std::vector<cv::Vec3f> test_cloud_eigen(cloud_icp_size);
    for(int i=0; i<cloud_icp_size; i+=1){
        test_cloud_eigen[i](0) = test_cloud[i*cloud_icp_step].x();
        test_cloud_eigen[i](1) = test_cloud[i*cloud_icp_step].y();
        test_cloud_eigen[i](2) = test_cloud[i*cloud_icp_step].z();
    }

    auto R_real_icp = cv::Matx33f(1,0,0,
                                  0,1,0,
                                  0,0,1);
    auto T_real_icp = cv::Vec3f(0,0,0);
    float px_ratio_match_inliers = 0.0f;
    float icp_dist = icpCloudToCloud(model_v_eigen, test_cloud_eigen, R_real_icp,
                                     T_real_icp, px_ratio_match_inliers, 1);

    icp_dist = icpCloudToCloud(model_v_eigen, test_cloud_eigen, R_real_icp,
                               T_real_icp, px_ratio_match_inliers, 2);

    icp_dist = icpCloudToCloud(model_v_eigen, test_cloud_eigen, R_real_icp,
                               T_real_icp, px_ratio_match_inliers, 0);
    timer.out("icp");

    cv::Mat t1(4,4,CV_32FC1,transformation.data());
    t1 = t1.t();

    cv::Mat t2(4,4,CV_32FC1, cv::Scalar(0));
    t2.at<float>(0,3) = T_real_icp[0];
    t2.at<float>(1,3) = T_real_icp[1];
    t2.at<float>(2,3) = T_real_icp[2];
    t2.at<float>(3,3) = 1;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            t2.at<float>(i,j) = R_real_icp(i,j);
        }
    }

    auto result = (t2*t1).inv();

    std::cout << "result: " << result << std::endl;
    std::cout << "icp_dist: " << icp_dist << std::endl;
    std::cout << "px_ratio_match_inliers: " << px_ratio_match_inliers << std::endl;

    waitKey(0);
}

int main(){
//    simple_test();
//    dataset_test();
    super4pcs_test();
    cout << "end" << endl;
    return 0;
}