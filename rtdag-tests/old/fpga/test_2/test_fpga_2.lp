Minimize
 obj: pow

Subject To
 - pow + 1.68586 y_0_0 + 1.19 y_1_0 + Pmax_0_0 + 0.028286 z_0_0_0_0_0 + 0.056572 z_0_1_0_0_0 + 0.042429 z_0_2_0_0_0 + 0.056572 z_0_3_0_0_0 + 0.028286 z_0_4_0_0_0 + Pmax_0_1 + 0.028286 z_0_0_0_1_0 + 0.056572 z_0_1_0_1_0 + 0.042429 z_0_2_0_1_0 + 0.056572 z_0_3_0_1_0 + 0.028286 z_0_4_0_1_0 + Pmax_0_2 + 0.028286 z_0_0_0_2_0 + 0.056572 z_0_1_0_2_0 + 0.042429 z_0_2_0_2_0 + 0.056572 z_0_3_0_2_0 + 0.028286 z_0_4_0_2_0 + Pmax_0_3 + 0.028286 z_0_0_0_3_0 + 0.056572 z_0_1_0_3_0 + 0.042429 z_0_2_0_3_0 + 0.056572 z_0_3_0_3_0 + 0.028286 z_0_4_0_3_0 + Pmax_1_0 + -0.0005 h_0_2_1_0_0 + -0.0005 h_0_3_1_0_0 = 0
 [ d_0_0 * di_0_0 ] = 1
 [ d_0_1 * di_0_1 ] = 1
 [ d_0_2 * di_0_2 ] = 1
 [ d_0_3 * di_0_3 ] = 1
 [ d_0_4 * di_0_4 ] = 1
 + x_0_0_0_0 + x_0_0_0_1 + x_0_0_0_2 + x_0_0_0_3 + x_0_0_1_0 = 1
 + x_0_1_0_0 + x_0_1_0_1 + x_0_1_0_2 + x_0_1_0_3 + x_0_1_1_0 = 1
 + x_0_2_0_0 + x_0_2_0_1 + x_0_2_0_2 + x_0_2_0_3 + x_0_2_1_0 = 1
 + x_0_3_0_0 + x_0_3_0_1 + x_0_3_0_2 + x_0_3_0_3 + x_0_3_1_0 = 1
 + x_0_4_0_0 + x_0_4_0_1 + x_0_4_0_2 + x_0_4_0_3 + x_0_4_1_0 = 1
 f_0_0 - d_0_0 = 0
 f_0_1 - f_0_0 - d_0_1 >= 0
 f_0_2 - f_0_1 - d_0_2 >= 0
 f_0_3 - f_0_0 - d_0_3 >= 0
 f_0_4 - f_0_3 - d_0_4 >= 0
 f_0_4 - f_0_2 - d_0_4 >= 0
 0.01 f_0_4 <= 1
 z_0_0_0_0_0 - x_0_0_0_0 <= 0
 z_0_0_0_0_0 - y_0_0 <= 0
 z_0_0_0_0_0 - x_0_0_0_0 - y_0_0 >= -1
 z_0_0_0_1_0 - x_0_0_0_1 <= 0
 z_0_0_0_1_0 - y_0_0 <= 0
 z_0_0_0_1_0 - x_0_0_0_1 - y_0_0 >= -1
 z_0_0_0_2_0 - x_0_0_0_2 <= 0
 z_0_0_0_2_0 - y_0_0 <= 0
 z_0_0_0_2_0 - x_0_0_0_2 - y_0_0 >= -1
 z_0_0_0_3_0 - x_0_0_0_3 <= 0
 z_0_0_0_3_0 - y_0_0 <= 0
 z_0_0_0_3_0 - x_0_0_0_3 - y_0_0 >= -1
 z_0_0_1_0_0 - x_0_0_1_0 <= 0
 z_0_0_1_0_0 - y_1_0 <= 0
 z_0_0_1_0_0 - x_0_0_1_0 - y_1_0 >= -1
 z_0_1_0_0_0 - x_0_1_0_0 <= 0
 z_0_1_0_0_0 - y_0_0 <= 0
 z_0_1_0_0_0 - x_0_1_0_0 - y_0_0 >= -1
 z_0_1_0_1_0 - x_0_1_0_1 <= 0
 z_0_1_0_1_0 - y_0_0 <= 0
 z_0_1_0_1_0 - x_0_1_0_1 - y_0_0 >= -1
 z_0_1_0_2_0 - x_0_1_0_2 <= 0
 z_0_1_0_2_0 - y_0_0 <= 0
 z_0_1_0_2_0 - x_0_1_0_2 - y_0_0 >= -1
 z_0_1_0_3_0 - x_0_1_0_3 <= 0
 z_0_1_0_3_0 - y_0_0 <= 0
 z_0_1_0_3_0 - x_0_1_0_3 - y_0_0 >= -1
 z_0_1_1_0_0 - x_0_1_1_0 <= 0
 z_0_1_1_0_0 - y_1_0 <= 0
 z_0_1_1_0_0 - x_0_1_1_0 - y_1_0 >= -1
 z_0_2_0_0_0 - x_0_2_0_0 <= 0
 z_0_2_0_0_0 - y_0_0 <= 0
 z_0_2_0_0_0 - x_0_2_0_0 - y_0_0 >= -1
 z_0_2_0_1_0 - x_0_2_0_1 <= 0
 z_0_2_0_1_0 - y_0_0 <= 0
 z_0_2_0_1_0 - x_0_2_0_1 - y_0_0 >= -1
 z_0_2_0_2_0 - x_0_2_0_2 <= 0
 z_0_2_0_2_0 - y_0_0 <= 0
 z_0_2_0_2_0 - x_0_2_0_2 - y_0_0 >= -1
 z_0_2_0_3_0 - x_0_2_0_3 <= 0
 z_0_2_0_3_0 - y_0_0 <= 0
 z_0_2_0_3_0 - x_0_2_0_3 - y_0_0 >= -1
 z_0_2_1_0_0 - x_0_2_1_0 <= 0
 z_0_2_1_0_0 - y_1_0 <= 0
 z_0_2_1_0_0 - x_0_2_1_0 - y_1_0 >= -1
 z_0_3_0_0_0 - x_0_3_0_0 <= 0
 z_0_3_0_0_0 - y_0_0 <= 0
 z_0_3_0_0_0 - x_0_3_0_0 - y_0_0 >= -1
 z_0_3_0_1_0 - x_0_3_0_1 <= 0
 z_0_3_0_1_0 - y_0_0 <= 0
 z_0_3_0_1_0 - x_0_3_0_1 - y_0_0 >= -1
 z_0_3_0_2_0 - x_0_3_0_2 <= 0
 z_0_3_0_2_0 - y_0_0 <= 0
 z_0_3_0_2_0 - x_0_3_0_2 - y_0_0 >= -1
 z_0_3_0_3_0 - x_0_3_0_3 <= 0
 z_0_3_0_3_0 - y_0_0 <= 0
 z_0_3_0_3_0 - x_0_3_0_3 - y_0_0 >= -1
 z_0_3_1_0_0 - x_0_3_1_0 <= 0
 z_0_3_1_0_0 - y_1_0 <= 0
 z_0_3_1_0_0 - x_0_3_1_0 - y_1_0 >= -1
 z_0_4_0_0_0 - x_0_4_0_0 <= 0
 z_0_4_0_0_0 - y_0_0 <= 0
 z_0_4_0_0_0 - x_0_4_0_0 - y_0_0 >= -1
 z_0_4_0_1_0 - x_0_4_0_1 <= 0
 z_0_4_0_1_0 - y_0_0 <= 0
 z_0_4_0_1_0 - x_0_4_0_1 - y_0_0 >= -1
 z_0_4_0_2_0 - x_0_4_0_2 <= 0
 z_0_4_0_2_0 - y_0_0 <= 0
 z_0_4_0_2_0 - x_0_4_0_2 - y_0_0 >= -1
 z_0_4_0_3_0 - x_0_4_0_3 <= 0
 z_0_4_0_3_0 - y_0_0 <= 0
 z_0_4_0_3_0 - x_0_4_0_3 - y_0_0 >= -1
 z_0_4_1_0_0 - x_0_4_1_0 <= 0
 z_0_4_1_0_0 - y_1_0 <= 0
 z_0_4_1_0_0 - x_0_4_1_0 - y_1_0 >= -1
 + y_0_0 = 1
 - u_0_0_0 + [  + 20 z_0_0_0_0_0 * di_0_0 ] <= 0
 - u_0_0_0 + [  + 20 z_0_4_0_0_0 * di_0_4 ] <= 0
 - u_0_0_0 + [  + 30 z_0_2_0_0_0 * di_0_2 + 40 z_0_3_0_0_0 * di_0_3 ] <= 0
 - u_0_0_0 + [  + 40 z_0_1_0_0_0 * di_0_1 + 40 z_0_3_0_0_0 * di_0_3 ] <= 0
 - u_0_0_1 + [  + 20 z_0_0_0_1_0 * di_0_0 ] <= 0
 - u_0_0_1 + [  + 20 z_0_4_0_1_0 * di_0_4 ] <= 0
 - u_0_0_1 + [  + 30 z_0_2_0_1_0 * di_0_2 + 40 z_0_3_0_1_0 * di_0_3 ] <= 0
 - u_0_0_1 + [  + 40 z_0_1_0_1_0 * di_0_1 + 40 z_0_3_0_1_0 * di_0_3 ] <= 0
 - u_0_0_2 + [  + 20 z_0_0_0_2_0 * di_0_0 ] <= 0
 - u_0_0_2 + [  + 20 z_0_4_0_2_0 * di_0_4 ] <= 0
 - u_0_0_2 + [  + 30 z_0_2_0_2_0 * di_0_2 + 40 z_0_3_0_2_0 * di_0_3 ] <= 0
 - u_0_0_2 + [  + 40 z_0_1_0_2_0 * di_0_1 + 40 z_0_3_0_2_0 * di_0_3 ] <= 0
 - u_0_0_3 + [  + 20 z_0_0_0_3_0 * di_0_0 ] <= 0
 - u_0_0_3 + [  + 20 z_0_4_0_3_0 * di_0_4 ] <= 0
 - u_0_0_3 + [  + 30 z_0_2_0_3_0 * di_0_2 + 40 z_0_3_0_3_0 * di_0_3 ] <= 0
 - u_0_0_3 + [  + 40 z_0_1_0_3_0 * di_0_1 + 40 z_0_3_0_3_0 * di_0_3 ] <= 0
 + y_1_0 = 1
 x_0_0_1_0 = 0
 x_0_1_1_0 = 0
 x_0_4_1_0 = 0
 bown_0_2_1_0 - h_0_3_1_0_0 >=  0
 x_0_2_1_0 = 1 -> d_0_2 - h_0_2_1_0_0 - bown_0_2_1_0 >= 0
 bown_0_3_1_0 - h_0_2_1_0_0 >=  0
 x_0_3_1_0 = 1 -> d_0_3 - h_0_3_1_0_0 - bown_0_3_1_0 >= 0
 m_1_0 - x_0_0_1_0 - x_0_1_1_0 - x_0_2_1_0 - x_0_3_1_0 - x_0_4_1_0 = 0
 s_1_0 - 0.5 m_1_0 <= 0
 s_1_0 - 0.2 m_1_0 >= - 0.2
 r_0_0_1_0_0 - z_0_0_1_0_0 <= 0
 r_0_0_1_0_0 - s_1_0 <= 0
 r_0_0_1_0_0 - z_0_0_1_0_0 - s_1_0 >= -1
 h_0_0_1_0_0 - 15.0376 z_0_0_1_0_0 - 23.664 r_0_0_1_0_0 = 0
 r_0_1_1_0_0 - z_0_1_1_0_0 <= 0
 r_0_1_1_0_0 - s_1_0 <= 0
 r_0_1_1_0_0 - z_0_1_1_0_0 - s_1_0 >= -1
 h_0_1_1_0_0 - 30.0752 z_0_1_1_0_0 - 23.664 r_0_1_1_0_0 = 0
 r_0_2_1_0_0 - z_0_2_1_0_0 <= 0
 r_0_2_1_0_0 - s_1_0 <= 0
 r_0_2_1_0_0 - z_0_2_1_0_0 - s_1_0 >= -1
 h_0_2_1_0_0 - 1.2765 z_0_2_1_0_0 - 23.664 r_0_2_1_0_0 = 0
 r_0_3_1_0_0 - z_0_3_1_0_0 <= 0
 r_0_3_1_0_0 - s_1_0 <= 0
 r_0_3_1_0_0 - z_0_3_1_0_0 - s_1_0 >= -1
 h_0_3_1_0_0 - 0.897615 z_0_3_1_0_0 - 23.664 r_0_3_1_0_0 = 0
 r_0_4_1_0_0 - z_0_4_1_0_0 <= 0
 r_0_4_1_0_0 - s_1_0 <= 0
 r_0_4_1_0_0 - z_0_4_1_0_0 - s_1_0 >= -1
 h_0_4_1_0_0 - 15.0376 z_0_4_1_0_0 - 23.664 r_0_4_1_0_0 = 0

Bounds
 20 <= d_0_0 <= 100
 0.01 <= di_0_0 <= 0.05
 0 <= f_0_0 <= 100
 40 <= d_0_1 <= 100
 0.01 <= di_0_1 <= 0.025
 0 <= f_0_1 <= 100
 1.2765 <= d_0_2 <= 100
 0.01 <= di_0_2 <= 0.783392
 0 <= f_0_2 <= 100
 0.897615 <= d_0_3 <= 100
 0.01 <= di_0_3 <= 1.11406
 0 <= f_0_3 <= 100
 20 <= d_0_4 <= 100
 0.01 <= di_0_4 <= 0.05
 0 <= f_0_4 <= 100
 0 <= u_0_0_0 <= 0.95
 0 <= Pmax_0_0 <= 0
 0 <= u_0_0_1 <= 0.95
 0 <= Pmax_0_1 <= 0
 0 <= u_0_0_2 <= 0.95
 0 <= Pmax_0_2 <= 0
 0 <= u_0_0_3 <= 0.95
 0 <= Pmax_0_3 <= 0
 0 <= m_1_0 <= 5
 0 <= bown_0_0_1_0 <= 100
 0 <= both_0_0_1_0 <= 100
 0 <= bown_0_1_1_0 <= 100
 0 <= both_0_1_1_0 <= 100
 0 <= bown_0_2_1_0 <= 100
 0 <= both_0_2_1_0 <= 100
 0 <= bown_0_3_1_0 <= 100
 0 <= both_0_3_1_0 <= 100
 0 <= bown_0_4_1_0 <= 100
 0 <= both_0_4_1_0 <= 100
 0 <= Pmax_1_0 <= 0

Binary
 x_0_0_0_0
 z_0_0_0_0_0
 x_0_0_0_1
 z_0_0_0_1_0
 x_0_0_0_2
 z_0_0_0_2_0
 x_0_0_0_3
 z_0_0_0_3_0
 x_0_0_1_0
 z_0_0_1_0_0
 x_0_1_0_0
 z_0_1_0_0_0
 x_0_1_0_1
 z_0_1_0_1_0
 x_0_1_0_2
 z_0_1_0_2_0
 x_0_1_0_3
 z_0_1_0_3_0
 x_0_1_1_0
 z_0_1_1_0_0
 x_0_2_0_0
 z_0_2_0_0_0
 x_0_2_0_1
 z_0_2_0_1_0
 x_0_2_0_2
 z_0_2_0_2_0
 x_0_2_0_3
 z_0_2_0_3_0
 x_0_2_1_0
 z_0_2_1_0_0
 x_0_3_0_0
 z_0_3_0_0_0
 x_0_3_0_1
 z_0_3_0_1_0
 x_0_3_0_2
 z_0_3_0_2_0
 x_0_3_0_3
 z_0_3_0_3_0
 x_0_3_1_0
 z_0_3_1_0_0
 x_0_4_0_0
 z_0_4_0_0_0
 x_0_4_0_1
 z_0_4_0_1_0
 x_0_4_0_2
 z_0_4_0_2_0
 x_0_4_0_3
 z_0_4_0_3_0
 x_0_4_1_0
 z_0_4_1_0_0
 s_1_0
 r_0_0_1_0_0
 r_0_1_1_0_0
 r_0_2_1_0_0
 r_0_3_1_0_0
 r_0_4_1_0_0
 y_0_0
 y_1_0

General

End
