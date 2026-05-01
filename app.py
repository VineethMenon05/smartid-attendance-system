from flask import Flask, render_template
import pandas as pd

app = Flask(__name__)

URL = "https://docs.google.com/spreadsheets/d/1tNjfeDknGADdyxme4xS-eMsl5NHyVWE-621O-ZcdpAA/gviz/tq?tqx=out:csv"

def get_data():
    try:
        df = pd.read_csv(URL)

        df.columns = df.columns.str.strip()

        # ✅ FIX NaN VALUES
        df = df.fillna("-")

        # ✅ OPTIONAL: clean Date & Time specifically
        if "Date" in df.columns:
            df["Date"] = df["Date"].astype(str).replace("nan", "-")

        if "Time" in df.columns:
            df["Time"] = df["Time"].astype(str).replace("nan", "-")

        print("DATA:\n", df.head())  # debug

        return df.to_dict(orient="records")

    except Exception as e:
        print("Error:", e)
        return []

@app.route("/")
def dashboard():
    data = get_data()

    # ✅ FIXED COUNTING (handles spaces + case)
    login_count = sum(
        1 for row in data 
        if str(row.get("Type", "")).strip().lower() == "login"
    )

    logout_count = sum(
        1 for row in data 
        if str(row.get("Type", "")).strip().lower() == "logout"
    )

    # 📊 STUDENT ANALYTICS (your code)
    from collections import Counter  

    names = [row["Name"] for row in data if row.get("Name")]  
    name_counts = Counter(names)  

    labels = list(name_counts.keys())  
    values = list(name_counts.values())

    return render_template(
        "dashboard.html",
        data=data,
        total_records=len(data),
        login_count=login_count,
        logout_count=logout_count,
        labels=labels,
        values=values
    )

if __name__ == "__main__":
    app.run(debug=True)
