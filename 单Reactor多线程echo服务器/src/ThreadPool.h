/******************************
*   author: yuesong-feng
*   
*
*
******************************/
#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    // void add(std::function<void()>);
    template<class F, class... Args>
    auto add(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;

};


//不能放在cpp文件，原因是C++编译器不支持模版的分离编译

//使用期物是为了可以返回值
//使用那么多奇怪的模板和变长变量是为了可以传入任意函数，实现参数自动绑定
template<class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    //返回类型用到了尾确定，值为一个期物，期物是由传入的函数返回的类型确定的
    //简写返回类型
    using return_type = typename std::result_of<F(Args...)>::type;
    //定义task以后续添加到tasks中，task由一个智能指针确定，指向了一个由bind绑定的可调用对象，即把args绑定到了f上
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
    //使用期物来访问异步操作的结果
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);

        // 当线程结束后不允许添加到线程池中
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    cv.notify_one();
    return res;
}
