import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Create transactions table for buying, selling, and history, plus index on id for fast search
db.execute("CREATE TABLE IF NOT EXISTS transactions (user_id INTEGER NOT NULL, user TEXT NOT NULL, \
            name TEXT NOT NULL, symbol TEXT NOT NULL, shares INTEGER NOT NULL, price NUMERIC NOT NULL, \
            datetime TEXT, FOREIGN KEY(user_id) REFERENCES users(id))")
db.execute("CREATE INDEX IF NOT EXISTS user_index ON transactions(user_id)")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    shares_owned = {}
    stocks = db.execute("SELECT symbol, shares FROM transactions WHERE user_id = ?", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    total = cash

    # Store symbol and shares of owned stocks in dict
    for i in stocks:
        symbol, shares = i["symbol"], i["shares"]
        shares_owned[symbol] = shares_owned.setdefault(symbol, 0) + shares

    # Get values of owned stocks
    for symbol, shares in shares_owned.items():
        name, price = lookup(symbol)["name"], lookup(symbol)["price"]
        total_price = shares * price
        total += total_price
        shares_owned[symbol] = (name, shares, usd(price), usd(total_price))

    return render_template("index.html", stocks=shares_owned, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide stock symbol", 400)
        elif not stock:
            return apology("must provide valid stock symbol", 400)
        elif not shares or not shares.isdigit():
            return apology("must provide valid number of shares", 400)

        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        updated_cash = rows[0]["cash"] - (int(float(shares)) * stock["price"])

        if updated_cash < 0:
            return apology("insufficient funds", 403)

        # Update Cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])

        # Update Transactions
        db.execute("INSERT INTO transactions (user_id, user, name, symbol, shares, price, datetime) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], rows[0]["username"], stock["name"], stock["symbol"], int(float(shares)), stock["price"], datetime.now().strftime("%m/%d/%Y %H:%M:%S"))

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY datetime DESC", session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))

        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        elif not stock:
            return apology("must provide valid stock symbol", 400)
        else:
            return render_template("quoted.html", name=stock["name"], price=usd(stock["price"]), symbol=stock["symbol"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username doesn't already exist, and passwords match
        if len(rows) != 0:
            return apology("username already exists!", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match!", 400)

        # Register user
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Redirect user to login page
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        owned = db.execute("SELECT * FROM transactions WHERE shares > ? AND user_id = ?", 0, session["user_id"])
        shares_owned = db.execute(
            "SELECT shares FROM transactions WHERE shares > ? AND user_id = ? AND symbol = ?", 0, session["user_id"], symbol)

        if not symbol:
            return apology("must pick stock symbol", 403)
        elif not shares or int(shares) < 1:
            return apology("must provide valid number of shares", 403)
        elif int(shares) > shares_owned[0]["shares"]:
            return apology("you do not own that many shares of " + symbol)

        stock = lookup(symbol)
        updated_cash = rows[0]["cash"] + (int(shares) * stock["price"])

        # Update Cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])

        # Update Transactions
        db.execute("INSERT INTO transactions (user_id, user, name, symbol, shares, price, datetime) VALUES (?, ?, ?, ?, ?, ?, ?)",\
                   session["user_id"], rows[0]["username"], stock["name"], stock["symbol"], -int(shares), stock["price"], datetime.now().strftime("%m/%d/%Y %H:%M:%S"))

        return redirect("/")
    else:
        owned = {}
        stocks = db.execute("SELECT symbol, shares FROM transactions WHERE user_id = ?", session["user_id"])

        for i in stocks:
            symbol, shares = i["symbol"], i["shares"]
            owned[symbol] = owned.setdefault(symbol, 0) + shares

        return render_template("sell.html", owned=owned)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Allow logged in user to change their password"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 403)
        elif not request.form.get("password"):
            return apology("must provide current password", 403)
        elif not request.form.get("new_pass"):
            return apology("must provide new password", 403)

        # Query database for username and hash password
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure user correctly implements new password
        if len(rows) != 1:
            return apology("invalid username", 403)
        elif not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("incorrect current password", 403)
        elif request.form.get("password") == request.form.get("new_pass"):
            return apology("new password must be different from old password", 403)
        elif request.form.get("new_pass") != request.form.get("confirmation"):
            return apology("passwords do not match", 403)

        # Change user password
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(
            request.form.get("new_pass")), session["user_id"])

        # Redirect user to login page
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change_password.html")