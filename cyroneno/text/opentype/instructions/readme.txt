使用指令控制字形


一、选择一种扫描转换模式
在准备使用指令控制 TrueType 字体时，需要做一项关键决定，就是选择扫描转换模式。字体设计者可以在
快速（fast）扫描模式和丢弃控制（dropout control）扫描转换模式这两种模式之中选择。该模式通过设置图形状态（Graphics State）
中的 scan_control 变量来控制。解释器考虑以下三个条件来决定是否使用丢弃控制模式：
  1. 字形是否被旋转？
  2. 字形是否被拉伸？
  3. 当前的 ppem 设置是否小于指定的 ppem 值？
当然，也可以直接关闭丢弃控制。


二、控制舍入
TrueType 解释器使用 round_state 来决定如何舍入。可使用指令设置这一位于图形状态中的变量。

使用指令集可设置一个预定义的舍入模式以将值舍入至网格、像素中心（半网格）或两者都。也可以指定值是向上还是向下舍入。如果你认为预定义的
舍入模式还不够用，可以使用 SROUND 指令，该指令提供了对舍入的完全控制，可以对值分阶段、设置阈值或设置范围来进行舍入。S45ROUND 指令
提供了与 SROUND 指令同样的自由度，不过它用于将值沿 x-y 平面上的 45 度轴移动。

有很多指令可以在移动任何点之前舍入值。MDRP、MIRP、MIAP、MDAP 或 ROUND 指令的效果依赖 round_state 和 control_value_cut_in。
ROFF 指令关闭舍入，不过指令可继续进行值的 cut-in。


三、点
轮廓点由坐标网格中的位置来指定，它可能位于曲线上，也可能不在曲线上。管理一个点意味着管理它的坐标和是否在曲线上。解释器使用区域（Zones）
和引用点来管理组成当前字形的点和其他特定的点。

1. 区域（Zones）
任何字体指令解释器引用到的点位于两个区域中的其中之一。换句话说，位于两个潜在构成字形的点集之一。第一个区域称为区域1（Z1），并且该区域
总是包含了当前解释中的字形。

第二个区域，区域0（Z0），用于临时存储不对应任何组成区域1中的字形的实际点的点坐标。区域0用于操作不存在于字形上的点，或者用于存储运算
过程中的中间点（也称为将亡区（twilight zone））。

配置表（profile table）指明了将亡点（twilight points）的最大个数，其索引从 0 到 maxTwilightPoints - 1，初始全部位于原点。
这些点可按照与区域1相同的规则来移动。

区域0中的点可通过 MIAP 和 MIRP 指令来移动，也可以将 gep0 指向 Z0。Z0 被频繁地使用以用于字体测量。

2. 区域指针
三个区域指针：gep0、gep1 和 gep2 用于引用 Z0 或 Z1。初始时，所有三个区域指针将指向区域1。

3. 引用点（Reference points）
区域指针用于访问一组点。引用点用于访问组中的特定点。解释器使用三个引用点：rp0、rp1 和 rp2。每一个均可以被设置为区域1中字形的轮廓点
或区域0中的任何点的编号。

总体上，区域指针和引用点属于图形状态。它们的值可以被指令修改。许多 TrueType 指令依赖图形区域指针和引用点来完成它们的功能。

4. 幽点（Phantom points）
微软光栅器 v1.7 和更新版本将总是在每个轮廓后面添加四个“幽点”来控制宽度和高度（v1.7 版本之前只添加两个幽点，只能控制宽度）。

如果一个字形的所有轮廓需要“n”个点（换句话说，从 0 到 n-1），然后缩放器将会添加点 n、n+1、n+2 和 n+3。点“n”将被加在字符原点，
点“n+1”将被放置在宽步进点（advance width point），点“n+2”将被放置在顶部原点，点“n+3”将被放置在高步进点。

所有四个幽点可由 TrueType 指令控制，可控制字形的轴承，宽步进和高步进（竖排版的情况）。轴承，宽步进和高步进将使用这些幽点来计算，称为
设备特定的宽度和高度（因为它们反映了宽和高根据设备特性而进行网格适配的结果）。设备特定的宽或高可能和线性缩放的宽或高
（由简单的缩放操作得到）相同，也可能不同，取决于应用于幽点的指令。

在将 TrueType 指令应用于高度相关的幽点之前，先用 GETINFO[] 检查光栅器是否为微软光栅器 v1.7 或更新版本。


四、计算距离
在底层，用指令控制一个字形意味着管理点之间的距离。管理距离的第一步是决定距离的重要性。举例来说，设置 CVT 的第一步涉及测量字体中关键点
之间的距离。测量字形轮廓两点之间的距离，虽然并不困难，但必须考虑到主要的因素。