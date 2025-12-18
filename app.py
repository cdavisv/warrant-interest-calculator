import tkinter as tk
from tkinter import messagebox
from datetime import datetime

# =========================
# Configuration
# =========================
DATE_FORMAT = "%m-%d-%y"

# =========================
# Utility Functions
# =========================
def calculate_days(start_date, end_date):
    start = datetime.strptime(start_date, DATE_FORMAT)
    end = datetime.strptime(end_date, DATE_FORMAT)
    return (end - start).days


def format_date_entry(event, entry):
    # Allow navigation and deletion keys
    if event.keysym in ("BackSpace", "Delete", "Left", "Right", "Tab"):
        return

    # Allow manual hyphen typing without interference
    if event.char == "-":
        return

    # Only respond to digit input
    if not event.char.isdigit():
        return

    old_value = entry.get()
    old_cursor = entry.index(tk.INSERT)
    old_hyphens = old_value.count("-")

    # Extract digits only
    digits = "".join(c for c in old_value if c.isdigit())

    # Limit to MMDDYY
    if len(digits) > 6:
        return

    # Build formatted date
    if len(digits) <= 2:
        new_value = digits
    elif len(digits) <= 4:
        new_value = digits[:2] + "-" + digits[2:]
    else:
        new_value = digits[:2] + "-" + digits[2:4] + "-" + digits[4:]

    new_hyphens = new_value.count("-")

    # Only update if formatting changed
    if new_value != old_value:
        entry.delete(0, tk.END)
        entry.insert(0, new_value)

        # Move cursor forward if a hyphen was auto-added
        new_cursor = old_cursor + (new_hyphens - old_hyphens)
        entry.icursor(new_cursor)




# =========================
# Main Application
# =========================
class WarrantCalculatorApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Libby Drainage District Warrant Calculator")
        self.geometry("520x420")
        self.resizable(False, False)
        self.show_main_menu()

    def clear_screen(self):
        for widget in self.winfo_children():
            widget.destroy()

    # =========================
    # Main Menu
    # =========================
    def show_main_menu(self):
        self.clear_screen()

        tk.Label(
            self,
            text="Libby Drainage District\nWarrant Calculator",
            font=("Arial", 16, "bold"),
            pady=25
        ).pack()

        tk.Button(
            self,
            text="Compute New Warrant",
            width=30,
            command=self.show_calculator
        ).pack(pady=10)

        tk.Button(
            self,
            text="Exit",
            width=30,
            command=self.quit
        ).pack(pady=10)

    # =========================
    # Calculator Screen
    # =========================
    def show_calculator(self):
        self.clear_screen()

        tk.Label(
            self,
            text="Warrant Calculation",
            font=("Arial", 14, "bold"),
            pady=10
        ).pack()

        form = tk.Frame(self)
        form.pack(pady=10)

        # Warrant Amount
        tk.Label(form, text="Warrant Amount ($):").grid(row=0, column=0, sticky="e", pady=5)
        self.warrant_entry = tk.Entry(form)
        self.warrant_entry.grid(row=0, column=1, pady=5)
        self.warrant_entry.focus_set()

        # Interest Rate
        tk.Label(form, text="Interest Rate (%):").grid(row=1, column=0, sticky="e", pady=5)
        self.interest_entry = tk.Entry(form)
        self.interest_entry.grid(row=1, column=1, pady=5)
        self.interest_entry.insert(0, "6.00")

        # Start Date
        tk.Label(form, text="Start Date (MM-DD-YY):").grid(row=2, column=0, sticky="e", pady=5)
        self.start_date_entry = tk.Entry(form)
        self.start_date_entry.grid(row=2, column=1, pady=5)

        # End Date
        tk.Label(form, text="End Date (MM-DD-YY):").grid(row=3, column=0, sticky="e", pady=5)
        self.end_date_entry = tk.Entry(form)
        self.end_date_entry.grid(row=3, column=1, pady=5)

        # Date auto-format bindings
        self.start_date_entry.bind("<KeyRelease>", lambda e: format_date_entry(e, self.start_date_entry))
        self.end_date_entry.bind("<KeyRelease>", lambda e: format_date_entry(e, self.end_date_entry))

        # Buttons
        btns = tk.Frame(self)
        btns.pack(pady=15)

        tk.Button(btns, text="Calculate", width=15, command=self.calculate).grid(row=0, column=0, padx=5)
        tk.Button(btns, text="Main Menu", width=15, command=self.show_main_menu).grid(row=0, column=1, padx=5)

        # Results
        self.results = tk.Label(self, text="", font=("Arial", 11), pady=15)
        self.results.pack()

    # =========================
    # Calculation Logic
    # =========================
    def calculate(self):
        try:
            warrant = float(self.warrant_entry.get())
            rate_percent = float(self.interest_entry.get())
            start_date = self.start_date_entry.get()
            end_date = self.end_date_entry.get()

            if warrant <= 0:
                raise ValueError("Warrant amount must be greater than zero.")

            if rate_percent <= 0 or rate_percent > 20:
                raise ValueError("Interest rate must be between 0 and 20 percent.")

            days = calculate_days(start_date, end_date)

            if days < 0:
                raise ValueError("End date must be after start date.")

            rate = rate_percent / 100
            interest = warrant * rate * (days / 365)
            total = warrant + interest

            self.results.config(
                text=(
                    f"Days: {days}\n"
                    f"Interest: ${interest:,.2f}\n"
                    f"Total Payback Amount: ${total:,.2f}"
                )
            )

        except ValueError as e:
            messagebox.showerror("Input Error", str(e))
        except Exception:
            messagebox.showerror(
                "Input Error",
                "Please ensure all values are valid and dates are in MM-DD-YY format."
            )


# =========================
# Run App
# =========================
if __name__ == "__main__":
    app = WarrantCalculatorApp()
    app.mainloop()
