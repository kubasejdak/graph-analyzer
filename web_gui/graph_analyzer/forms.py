from django import forms

class AnalyzeForm(forms.Form):
	path = forms.CharField()
	output = forms.CharField(initial = "DatabaseOutput")