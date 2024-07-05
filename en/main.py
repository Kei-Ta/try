import matplotlib.pyplot as plt

# データの定義
labels = ['生成AIやLLMやAI関連セッション', 'その他のセッション']
sizes = [35.19, 64.81]
colors = ['red', 'blue']

# 円グラフの作成
plt.pie(sizes, labels=labels, colors=colors, autopct='%1.1f%%', startangle=90)
plt.axis('equal')  # 円を真円に保つ
plt.title('セッションの割合')

# グラフの表示
plt.show()