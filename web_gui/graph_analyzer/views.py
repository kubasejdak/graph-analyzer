# Create your views here.

from django.shortcuts import render_to_response

def show_home(request):
	return render_to_response("home.html", {"version": "0.1"})

	