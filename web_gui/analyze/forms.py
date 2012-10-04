from django import forms

class AnalyzeForm(forms.Form):
	file  = forms.CharField(max_length = 100)