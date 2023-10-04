#include"base_pic_process.h"
#include"Triangular_structure.h"
//extern int pixel = 150;




	int MatrixHandler::AccumulateUpperTrangle(const Mat& a, int x ) {
		int m = a.rows, n = a.cols;
		int sum = 0;
		for (int i = 0; i < m; i++) {
			for (int j = max(0, i + x); j < n; j++) {
				sum += a.at<uchar>(i, j);
			}
		}
		return sum;
	}

	// 下三角求和
	int MatrixHandler::AccumulateLowerTrangle(const Mat& a, int x ) {
		int m = a.rows, n = a.cols;
		int sum = 0;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j <= (i + x); j++) {
				sum += a.at<uchar>(i, j);
			}
		}
		return sum;
	}



	Mat ImgMatrixHandler::NormalizeMatrix(const cv::Mat& pic) {
		cv::Mat mat;
		if (normalized_pixel > 0) {
			// 如果 normalized_pixel > 0，则重置像素大小
			Size dsize = Size(normalized_pixel, normalized_pixel);
			cv::resize(pic, mat, dsize, 0, 0, INTER_AREA);
		}
		else {
			mat = pic.clone();
		}
		int m = mat.rows, n = mat.cols;
		// 根据门限值，二值化处理
		for (int i = 0; i < mat.rows; ++i) {
			for (int j = 0; j < mat.cols; ++j) {
				// 0 - 255    黑 - 白
				if (mat.at<uchar>(i, j) >= threads_hold) {
					mat.at<uchar>(i, j) = 1;
				}
				else {
					mat.at<uchar>(i, j) = 0;
				}
			}
		}
		return mat;
	}




	// 照抄老师Matlab代码
	void TriangularHandler::Encode(const Mat& picture, vector<Edge>& edges,
		vector<Trangle>& trangles, int& num_of_point, int& num_of_edge,
		int& num_of_tran) {
		// 数组预先扩容，避免pushback之后容量不够，频繁申请内存
		trangles.reserve(5120);
		edges.reserve(5120);

		Mat f = picture;
		Mat mark = f;
		int size = picture.rows;
		int n = MyLogTwo(size);
		cout << "size = " << size << " n = " << n << endl;
		int num_of_upper_triangular = 0;
		int num_of_lower_triangular = 0;
		// matlab [1, size] cpp [0, size - 1]
		int step_tem = step;
		for (int var_sp_x = 1; var_sp_x <= size - 1; var_sp_x++) {
			for (int var_sp_y = 1; var_sp_y <= size - 1; var_sp_y++) {
				for (step=step_tem; step >= 1; step--) {
					if ((var_sp_x + step <= size) && (var_sp_y + step <= size)) {
						// f = mark;
						Mat temp_f = mark(Range(MatLab(var_sp_x), (var_sp_x + step)),
							Range(MatLab(var_sp_y), (var_sp_y + step)));
						int size_temp = step + 1;
						for (int k = 0; k <= (size_temp - 2); k++) {
							auto sum = AccumulateUpperTrangle(temp_f, k);
							if (sum == 0) {
								num_of_upper_triangular = num_of_upper_triangular + 1;
								int x1 = 1;
								int y1 = k + 1;
								int x2 = size_temp - k;
								int y2 = size_temp;
								int x1_abs = x1 + var_sp_x - 1;
								int y1_abs = y1 + var_sp_y - 1;
								int x2_abs = x2 + var_sp_x - 1;
								int y2_abs = y2 + var_sp_y - 1;
								// std::vector<T>::push_back()
								trangles.push_back(
									make_tuple(0, x1_abs, y1_abs, x2_abs, y2_abs));
								for (int i = x1; i <= x2; i++) {
									for (int j = (i + k); j <= y2; j++) {  // 遍历上三角区域。
										mark.at<uchar>(MatLab(i + var_sp_x - 1),
											MatLab(j + var_sp_y - 1)) = 8;
									}
								}
							}
							break;
						}
						for (int k = 0; k >= (-size_temp + 2); k--) {
							// 对下三角操作，同上。
							auto sum = AccumulateLowerTrangle(temp_f, k);
							if (sum == 0) {
								num_of_lower_triangular = num_of_lower_triangular + 1;
								int x1 = 1 - k;
								int y1 = 1;
								int x2 = size_temp;
								int y2 = size_temp + k;
								int x1_abs = x1 + var_sp_x - 1;
								int y1_abs = y1 + var_sp_y - 1;
								int x2_abs = x2 + var_sp_x - 1;
								int y2_abs = y2 + var_sp_y - 1;
								trangles.push_back(
									make_tuple(1, x1_abs, y1_abs, x2_abs, y2_abs));
								for (int i = x1; i <= x2; i++) {
									for (int j = 1; j <= i + k; j++) {
										mark.at<uchar>(MatLab(i + var_sp_x - 1),
											MatLab(j + var_sp_y - 1)) = 9;
									}
								}
							}
							break;
						}
					}
				}
			}
		}
		// part two

		f = mark;
		int blackNodeNum = 0;
		int line_num = 0;
		int point_num = 0;
		int Width0 = 1;
		int Length0 = 1;
		for (int x = 1; x <= size; x++) {
			for (int y = 1; y < size; y++) {
				int sp_y = y;
				int sp_x = x;

				if (f.at<uchar>(MatLab(x),
					MatLab(y)) == 0) {  // 不包于上下三角形中，且为空点。
					int Length = 1;
					int Width = 1;
					int MaxArea = 0;
					int last = 0;
					int MaxLength = 0;
					int MaxWidth = 0;
					for (int j = y; j <= size; j++) {
						Length0 = 0;
						for (int k = x; k <= size; k++) {
							if ((j != y) && (k - sp_x + 1 > last)) {
								break;
							}
							if (f.at<uchar>(MatLab(k), MatLab(j)) != 0) {
								break;
							}
							Length0 = Length0 + 1;
						}

						if (j == y) {
							last = Length0;
						}
						if (Length0 < last) {
							last = Length0;
						}
						if (f.at<uchar>(MatLab(x), MatLab(j)) != 0) {
							break;
						}
						Width0 = Width0 + 1;
						int Area = last * (j - sp_y + 1);
						Length = last;
						Width = j - sp_y + 1;
						if (Area > MaxArea) {
							MaxArea = Area;
							MaxLength = last;
							MaxWidth = j - sp_y + 1;
						}
					}

					if (MaxLength * MaxWidth == 1) {
						// 统计点数量
						point_num = point_num + 1;
					}
					else {
						// 统计边数量
						line_num = line_num + 1;
					}
					blackNodeNum = blackNodeNum + 1;
					edges.push_back(make_tuple(x, y, MaxLength, MaxWidth));
					for (int a = x; a <= x + MaxLength - 1; a++) {
						for (int b = y; b <= y + MaxWidth - 1; b++) {
							f.at<uchar>(MatLab(a), MatLab(b)) = 7;
						}
					}
				}
			}
		}

		// 引用返回信息
		num_of_point = point_num;
		num_of_edge = line_num;
		num_of_tran = num_of_upper_triangular + num_of_lower_triangular;
	}

	// 照抄老师Matlab代码
	void TriangularHandler::Decode(Mat& res, const vector<Edge>& edges,
		const vector<Trangle>& trangles) {
		// 全部像素置为255（白色）
		res = 255;

		for (const auto& tuple_trangle : trangles) {
			/*
			template< std::size_t I, class... Types >
			typename std::tuple_element<I, tuple<Types...> >::type&
			  get( tuple<Types...>& t ) noexcept;
			*/
			int flag = get<0>(tuple_trangle);
			int x1_abs = get<1>(tuple_trangle);
			int y1_abs = get<2>(tuple_trangle);
			int x2_abs = get<3>(tuple_trangle);
			int y2_abs = get<4>(tuple_trangle);

			switch (flag) {
			case 0: {
				int step = 0;
				for (int i = x1_abs; i <= x2_abs; i++) {
					for (int j = y1_abs + step; j <= y2_abs; j++) {
						res.at<uchar>(MatLab(i), MatLab(j)) = set_up_trangle;
					}
					step = step + 1;
				}
			} break;
			case 1: {
				int step = 0;
				for (int i = x1_abs; i <= x2_abs; i++) {
					for (int j = y1_abs; j <= y1_abs + step; j++) {
						res.at<uchar>(MatLab(i), MatLab(j)) = set_lo_trangle;
					}
					step = step + 1;
				}
			} break;
			}
		}
		for (const auto& edge : edges) {
			int x = get<0>(edge);
			int y = get<1>(edge);
			int MaxLength = get<2>(edge);
			int MaxWidth = get<3>(edge);

			for (int a = x; a <= x + MaxLength - 1; a++) {
				for (int b = y; b <= y + MaxWidth - 1; b++) {
					res.at<uchar>(MatLab(a), MatLab(b)) = set_edge;
				}
			}
		}
	}



