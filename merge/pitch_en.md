### A Million-Commit Story: How We Handle History Queries in Arc VCS

(1) Intro
At Yandex we do love monorepos:
according to our metrics, monorepos allow users to write code faster and ease code quality maintenance,
thus shortening time to market.

In order to make everything work smoothly, we have implemented our own git-like VCS
and we actively push the users towards trunk-based development.

This works — and people write a lot of code.
Today we merge up to 15000 commits into trunk every workday,
and we have recently landed r20'000'000 into our trunk branch.

(2) The problem
The history of Arc VCS development is a history of struggle against algorithmic complexity.
None of the classic git approaches works at our scale.
As soon as we implement a well-known algorithm, it stops working due to the repository growth.
We had to:
- examine how other VCS handle history queries,
- scale git approaches to suit our workloads,
- avoid unnecessary computations and reduce complexity where possible.

(3) Our approach
I will cover three aspects of working with such a long commit history:
- How we display commit log and why classic BFS does not work at our scale,
  how we first sped it up, and then abandoned it entirely
- How we optimize file log by using server side index (and why it is log_index_v3)
- How we built a blame index to speed up `arc praise` results

(4) Why this matters
You will learn how `git` that you use every day works — and how quickly its approaches stop working at scale.
I will cover classic algorithm complexities and how to speed them up.
I will show you one of the oldest lines in Yandex codebase, which survived 20 years without changes
so that everyone will know what ideal code looks like.
