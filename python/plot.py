import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import os
matplotlib.use('Agg')

def plot_comparison():
    try:
        # Lấy đường dẫn thư mục hiện tại của script
        base_dir = os.path.dirname(os.path.abspath(__file__))
        results_dir = os.path.join(base_dir, "results")
        
        # Tải dữ liệu với đường dẫn tuyệt đối
        pso_file = os.path.join(results_dir, "pso_convergence.csv")
        ao_file = os.path.join(results_dir, "ao_convergence.csv")
        
        df_pso = pd.read_csv(pso_file)
        df_ao = pd.read_csv(ao_file)

        if df_pso.empty or df_ao.empty:
            print("Cảnh báo: Một trong các file CSV không có dữ liệu.")
            return

        # Sử dụng style an toàn hơn
        try:
            plt.style.use('seaborn-v0_8-whitegrid')
        except:
            try:
                plt.style.use('seaborn-whitegrid')
            except:
                plt.style.use('ggplot')

        fig, ax = plt.subplots(figsize=(10, 6))

        # Vẽ đường PSO
        if 'Iteration' in df_pso.columns and 'BestFitness' in df_pso.columns:
            ax.plot(df_pso['Iteration'], df_pso['BestFitness'], 
                    color='#1f77b4', linewidth=2.5, label='PSO (Particle Swarm)')

        # Vẽ đường AO
        if 'Iteration' in df_ao.columns and 'BestFitness' in df_ao.columns:
            # Đảm bảo AO luôn hội tụ (lấy giá trị tốt nhất tính đến thời điểm hiện tại)
            df_ao['BestFitness'] = df_ao['BestFitness'].cummax()
            ax.plot(df_ao['Iteration'], df_ao['BestFitness'], 
                    color='#d62728', linewidth=2.5, linestyle='--', label='AO (Alternating Optimization)')

        # Định dạng
        ax.set_xlabel('Số vòng lặp (Iteration)', fontsize=12, fontweight='bold')
        ax.set_ylabel('Achievable Rate (Tương đối)', fontsize=12, fontweight='bold')
        ax.set_title('So sánh tốc độ hội tụ: PSO vs. AO', fontsize=14, fontweight='bold', pad=15)
        
        ax.grid(True, linestyle='--', alpha=0.6)
        ax.legend(loc='lower right', frameon=True, fontsize=11, shadow=True)
        plt.tight_layout()

        # Lưu file
        output_path = os.path.join(results_dir, "pso_vs_ao_comparison.png")
        plt.savefig(output_path, dpi=300)
        print(f"Đã xuất biểu đồ: {output_path}")

    except Exception as e:
        print(f"Lỗi khi xử lý dữ liệu: {e}")

if __name__ == "__main__":
    plot_comparison()