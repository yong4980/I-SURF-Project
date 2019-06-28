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

![MARS_Overview](./img/MARS_Overview.png)



1. 