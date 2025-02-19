from database import Database
from flask import (
    send_from_directory,
    request,
    jsonify,
    Flask
)
import os

app = Flask(
    __name__,
    static_folder="public"
)
db = Database()

@app.route("/", methods=["GET"])
def index():
    return send_from_directory(os.path.join(
        app.root_path, "public"
    ), "index.html")

@app.route("/bin/<id>", methods=["GET"])
def get_bin(id):
    bin = db.get_bin(id)
    if bin:
        return jsonify({
            "status": True,
            "bin": bin
        })
    else:
        return jsonify({
            "status": False,
            "bin": "Not Found"
        })

@app.route("/bin", methods=["POST"])
def post_bin():
    data = request.get_json()
    bin = data.get("bin")

    if not(bin):
        return jsonify({
            "status": False,
            "error": "Missing parameters!"
        })

    id = db.create_bin(bin)
    if id:
        return jsonify({
            "status": True,
            "id": id
        })

if __name__ == "__main__":
    app.run(debug=True)
