### Million Commits Story: How we handle history queries in Arc VCS

(1) Intro
In Yandex we do love monorepos:
according the what we measure, monorepos allow users to write code faster and ease its quality maintaintenance,
thus shortening time to market.

In order to make everything work smoothly, we have implemented out own git-like VCS
and we actively push the users towards trunk-based development.

Our efforts allow people to write code A LOT.
Today we merge up to 15000 commits into trunk every workday,
and we have recently landed r20'000'000 into our trunk branch.

(2) The problem
Arc VCS history is a struggle against algorithmic complexity.
None of git approaches works at our scale.
Once we implemented a well-known algorithms, it stopped working due to exponential growth of the repository.
We had to:
- study how other VCS handle history queries,
- scale git approarches to suit our workloads,
- avoid unnecessary computations and save complexity where possible.

(3) Our approach
I will cover three aspects of working with such history:
* How we handle branch history / commit graph and why neither classic BFS, nor 
* How we optimize file log by using server side index (and why it is log_index_v3)
* How we compute blame / annotate result

(4) Why this matters
I will speak on the things that prevent git from achieving our scale.
I will cover classic algorithms complexities and how to speed them up.
I will show one of the oldest lines in Yandex codebase, so everyone would know
how the ideal code should like like to keep it unchanged for 20 years.