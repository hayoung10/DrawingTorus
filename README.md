# DrawingTorus
Computer Graphics First Assignment - Drawing Torus  
2019.11.08 컴퓨터 그래픽스 과제  
여러 조건의 Torus 그리기  


> 환경 : Visual Studio 2015  
> 요구사항
- Generate torus by revolving a circle around the y-axis
  - 18 curve points for the circle
  - 36 steps for sweeping around the y-axis
- Draw the torus with the quads and wireframe
- Two-sided constant shading
  - Blue for outside
  - Red for inside
- Keyboard input  
  - Drawing the torus
    | Key | action |  
    | ----- | ----- |  
    | 1 | Drawing 36x18 data points |
    | 2 | Draw the wireframe only |
    | 3 | Draw the quads only |
    | 4 | Draw the quads and the wireframe |
  - Two-sided constant shading
    | Key | action |  
    | ----- | ----- | 
    | 5 | Draw the normal vectors of the polygons |
    | 6 | Two-sided constant shading with the wireframe |
  - Sweep angle control around 2 different axes  
    | Key | action |  
    | ----- | ----- |  
    | a | increase the sweep angle by 10º around the y-axis |  
    | s | decrease the sweep angle by -10º around the y-axis |  
    | j | increase the sweep angle by 20º around the z-axis |
    | k | decrease the sweep angle by -20º around the z-axis |
