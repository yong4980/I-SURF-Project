## A summary of Reflective On-Chip Resource Management Policies for Energy-Efficient Heterogeneous Multiprocessors

### chapter 1

Moore's law is coming to an end. Because the operating voltage has not been significantly decreasing, thus it's no longer possible to keep packing more transistors in the same chip area. ("power wall"). 

Heterogeneous processing elements extract more performance with a limited power envelope and avoid hitting the power wall.

GPUs consume low energy per calculation and are particularly energy efficient for applications with very high and regular parallelism.

* what is big.LITTLE? - big.LITTLE technology is a heterogeneous processing architecture that uses two types of processor. ”LITTLE” processors are designed for maximum power efficiency while ”big” processors are designed to provide maximum compute performance. With two dedicated processors, the big.LITTLE solution is able to adjust to the dynamic usage pattern for smartphones, tablets and other devices.

* what is HMP? - HMP is Heterogeneous Multi Processing

Important 2 questions

1. What is the most power efficient core type to execute the application without violating its performance requirements?
2. Which option is more power-efficient for the current application : an out-of-order core at a lower frequency an inorder core at a higher frequency?

* what is out-of-order core (OoO core)? - out-of-order execution (or more formally dynamic execution) is a paradigm used in most high-performance central processing units to make use of instruction cycles that would otherwise be wasted. In this paradigm, a processor executes instructions in an order governed by the availability of input data and execution units, rather than by their original order in a program. In doing so, the processor can avoid being idle while waiting for the preceding instruction to complete and can, in the meantime, process the next instructions that are able to run immediately and independently.

GTS(Global Task Scheduling) migrates tasks between high-performance and low-power core types when the task load reaches a certain threshold. But, this policy are customized for certain heterogeneous without being adaptable to other platforms. Furthermore, they exclusively focus on  task mapping/scheduling decisions and do not provide any coordination with the underlying power management subsystems.

Let's know a vision of a holistic approach for performing resource allocation decisions and power management by leveraging concepts from reflective software.

In this system, performing resource management actuation considering both sensing information to assess the current system state, as well as models to predict the outcome of the actuation before performing it.

![MARS_Overview](./img/MARS_Overview.PNG)



### Chapter 2

* what is reflection? - A reflective system usually maintain a representation of itself within the underlying system, which is used for reasoning.

Notice : The reflection mechanisms used in these works impose a rather heavy workload that would cause significant performance deterioration in devices with limited resources and there is always a trade-off issue between performance and scope of adaptability.

MIMO(Multiple-Output Multiple-Input) is a robust solution and provides a systematic way incorporate new resources into the design. However, MIMO is not scalable[82] and cannot handle discrete system logic. 

* what is ODA? - all propose task mapping heuristics that use offline-trained models to predict the performance and/or power of a new task mapping based on current sensed data.
* what is the difference between Reflection and ODA? - In ODA decisions are made based either on past observation or predictions made from past observations. These predictions do not consider future action or events that could happen until the next iteration of the ODA loop. 

![Difference ODA_loops and MARS](./img/difference_ODA_MARS.PNG)



Self-adaptive software can be defined as "software that evaluates its own behavior and changes behavior when the evaluation indicates that it is not accomplishing what the software is intended to do, or when better functionality or performance is possible".

This thesis dose not address self-adaptive systems directly, but an infrastructure for system introspection and reflective behavior is an important building block for such systems, which highlights the potentials contributions of MARS in this context.



### chapter 3

![MARS Framework](./img/MARS_framework.PNG)

MARS is mainly composed of four parts

1. Sensors and Actuators. - The sensed data that consists of performance counters and other sensory information collected to assess the current system state and to characterize workloads. 

2. Resource management policies implemented by MARS's users.

3. Reflective system model used by the policies to make informed decisions. The reflective model has the following subcomponents

   (a) Models for policies implemented by the underlying OS kernel used for coordinating decisions taken within MARS with decisions taken by the OS.

   (b) Models for user policies, that are automatically instantiated from any policy defined within MARS.

   (c) The baseline performance/power model. This model takes as input the predicted actuations generated from the policy models and produces predicted sensed data.

4. The high-level policy manager that is responsible for reconfiguring the system by adding, removing, or swapping policies to better achieve the current system goal.



MARS interfaces

![MARS Interfaces UML diagram](./img/MARS_interfaces_UML.PNG)

In summary, the reflection flow works as follows

1. Policy models predict which actuations are going to be performed in the future. If no models or other policies were registered, we just assume the latest values read from actuationVal/tryActuationVal won't be changed during the future sensing window.

2. The actuations predicted by step (1) are fed to Perf_Power_Model. The metrics returned are then used to predict the requested information returned by senseif.



MARS, the reflection mechanisms are tailored toward on-chip resource management to reduce the overheads. MARS's overheads come from mainly three sources

1. The sensing interface
2. The baseline performance/power model
3. The complexity of the policies used

what is High-level policy manager? - The motivation behind the high-level policy manager is to provide better resource management autonomy in response to changing system goals or execution scenarios. MARS was designed keeping in mind the presence of a high-level manager.



### chapter 4

we use the Linux-based implementation of the virtual interface. Each interface must provide the mechanisms necessary for the implementation of the sense and actuate functions, as well as the periodic triggering of the policy functions. 

![Linux toolchain Overview](./img/Linux_toolchain.PNG)

##### Evaluation Platforms

ODROID-XU3

- specification : Samsung Exynose5422 HMP(8-core big.LITTLE-based HMP), per-cluster power sensors, temperature sensors

GEM5

- defined - The gem5 simulator is a modular platform for computer-system architecture research, encompassing system-level architecture as well as processor microarchitecture.
- This allowed us to evaluate MARS on HMP platforms with more than two core types.
- This project only use FS(Full System Mode), in which a complete operating system boots on top of gem5 and system calls are executed natively.



##### Interface overheads 

![Interface Overheads](./img/Interface_overheads.PNG)

There is larger increase in overhead when we compare the vanilla kernel vs MARS(100ms window), as opposed to MARS(100ms window) vs MARS(10ms window). This is due to the fact that MARS's  kernel module always samples performance counter swtiches regardless of the window length. MARS's overhead is very small(0.37%)



