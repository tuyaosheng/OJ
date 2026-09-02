**汉诺塔**是递归的"镇山之宝"，几乎每本算法书都会讲它。

有三根柱子，分别叫 **A**、**B**、**C**。开始时，**A 柱**上从下到上套着 n 个圆盘，**大的在下、小的在上**（像一座塔）：

<img src="/public/upload/hanoi/hanoi_start.png" alt="初始状态：盘子从大到小叠在第一根柱子上" style="max-width:100%;border-radius:8px;">

现在要把这 n 个盘子**全部移到 C 柱**上，规则是：

1. **每次只能移动一个盘子**（从某根柱子的最顶端，移到另一根柱子的最顶端）；
2. **任何时候，都不能把大盘子压在小盘子上面**。

<div style="display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:8px;margin:10px 0;">
<img src="/public/upload/hanoi/hanoi_rule1.png" alt="一次只能移动一个盘子" style="width:100%;border-radius:8px;">
<img src="/public/upload/hanoi/hanoi_rule2.png" alt="必须保持小盘在上" style="width:100%;border-radius:8px;">
<img src="/public/upload/hanoi/hanoi_goal.png" alt="目标：全部移到另一根柱子" style="width:100%;border-radius:8px;">
</div>

<p style="color:#808695;font-size:14px;">（配图出自游戏《原神》里的汉诺塔小谜题——规则一模一样：一次只能移一个、必须小的在上、全部移到另一根柱子。）</p>

请输出**移动的总步数**，以及**每一步怎么移**。

**为什么递归是唯一的出路？**

想直接用循环模拟？那你得时刻盯着三根柱子上的盘子分布，逻辑会复杂到爆炸。

但用递归——**只需三行**。因为汉诺塔有一个极其优美的递归结构，一旦看破，问题瞬间瓦解。

<!-- 注意：图片源文件在本目录 images/ 下，需部署到 OnlineJudgeDeploy/data/backend/public/upload/hanoi/ 才能通过 /public/upload/hanoi/ URL 访问 -->
