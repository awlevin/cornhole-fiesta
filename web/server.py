from flask import Flask, request, render_template
import os, csv

PORT = os.environ["PORT"]

app = Flask(__name__)
app.config.from_object(__name__)

@app.route("/")
def testMethod():
    return render_template('index.html')

if __name__ == "__main__":
    app.run(debug=True, port=int(PORT))
