'----------------------------------------------------------------------
'FILE DESCRIPTION: SAMPLE.VBS is a collection of sample VrmlPad macros.
'----------------------------------------------------------------------

'------------------------------------------------------------
'Inserts all fields of the selected node with default values.
'------------------------------------------------------------

BindCommand "Complete_All", "Inserts all fields of the node",, "Alt+C"

Sub Complete_All
	Set ent = CurrentEntity
	If ent Is Nothing Then Exit Sub
	If ent.EntityType = vpNode Then
		BeginOperation "Complete All"
		For Each fld In ent.Fields
			fld.Implicit = False
		Next
		EndOperation
	End If
End Sub

'-------------------------------------------------------
'Prompts for a node name and selects the specified node.
'-------------------------------------------------------

Sub Go_To_Node
	nn = InputBox("Enter a node name:")
	If nn = "" Then Exit Sub
	On Error Resume Next
	Set node = Nodes(nn)
	If node Is Nothing Then
		Set node = CurrentContext.Nodes(nn)
	End If
	If node Is Nothing Then
		MsgBox "Can't find the node '" + nn + "'"
	Else
		node.Range(vprnName).Select
	End If
End Sub

'-----------------------------------------------------------------
'Enumerates all faces in the document and in the selected faceset.
'-----------------------------------------------------------------

BindCommand "Count_Faces", "Enumerates all faces", "Count &Faces..."
BindPopup "Count_Faces", "Count &Faces...", "IndexedFaceSet, IndexedFaceSet.*"

Function FacesInFaceset (fs)
	count = 0
	newface = True
	For Each ind In fs("coordIndex").Value
		If ind < 0 Then
			newface = True
		ElseIf newface Then
			count = count + 1
			newface = False
		End If
	Next
	FacesInFaceset = count
End Function

Sub Count_Faces
	count = 0
	For Each fs In StdProtos("IndexedFaceSet").Instances
		count = count + FacesInFaceset(fs)
	Next
	str = "Total " & count & " faces"

	Set ent = CurrentEntity
	Do Until ent Is Nothing
		If ent.EntityType = vpNode Then
			If ent.TypeName = "IndexedFaceSet" Then
				str = str & vbCrLf & FacesInFaceset(ent)
				str = str + " in the selected faceset"
				Exit Do
			End If
		End If
		Set ent = ent.Owner
	Loop
	MsgBox str
End Sub

'------------------------------------------------------------
'Wraps the selected node by Group, Transform or Anchor nodes.
'------------------------------------------------------------

Sub WrapNodeBy (env)
	Set node = CurrentEntity
	If node Is Nothing Then Exit Sub
	If node.EntityType <> vpNode And _
		node.EntityType <> vpNodeRef Then Exit Sub
	Set owner = node.Owner
	If owner Is Nothing Then
		Set coll = RootNodes
	ElseIf owner.EntityType = vpProto Then
		Set coll = owner.RootNodes
	ElseIf (owner.EntityType = vpField Or _
			owner.EntityType = vpFieldDecl) And _
			owner.Type = vpfMFNode Then
		Set coll = owner.Value
	Else
		MsgBox "Can't wrap this node"
		Exit Sub
	End If
	BeginOperation "Wrap Node"
	Dim nn
	nn = node.name
	Set group = coll.Add(env, node.Range)("children")
	group.Add node
	node.DeleteInstance
	Set node = group(group.Count)
	If node.EntityType = vpNode Then node.name = nn
	EndOperation
End Sub

BindCommand "WrapNodeByGroup", "Wraps the selected node by Group", "&Wrap by|&Group"

Sub WrapNodeByGroup
	WrapNodeBy("Group")
End Sub

BindCommand "WrapNodeByTransform", "Wraps the selected node by Transform", "&Wrap by|&Transform"

Sub WrapNodeByTransform
	WrapNodeBy("Transform")
End Sub

BindCommand "WrapNodeByAnchor", "Wraps the selected node by Anchor", "&Wrap by|&Anchor"

Sub WrapNodeByAnchor
	WrapNodeBy("Anchor")
End Sub

'------------------------------------------------------
'Converts Box, Cone or Cylinder node to IndexedFaceSet.
'------------------------------------------------------

BindCommand "ConvertToFaceset", "Converts Box, Cone or Cylinder to IndexedFaceSet", "To Face&set"
BindPopup "ConvertToFaceset", "Convert To Face&set", "Box, Cone, Cylinder"

Sub Box2Faceset (ByVal node, ByRef coord, ByRef index)
	size = node("size")
	ReDim coord(7,2)
	For i = 0 To 7
		coord(i, 0) = (.5 - (i And 4)/4) * size.x
		coord(i, 1) = (.5 - (i And 2)/2) * size.y
		coord(i, 2) = (.5 - (i And 1)) * size.z
	Next
	index = Array(4,0,1,5,-1, 7,3,2,6,-1, 6,2,0,4,-1,_
				  2,3,1,0,-1, 3,7,5,1,-1, 7,6,4,5)
End Sub

Sub Cone2Faceset (ByVal node, ByRef coord, ByRef index)
	Const n = 20
	h = node("height")/2
	r = node("bottomRadius")
	side = node("side")
	bottom = node("bottom")
	If bottom Then k = n Else k = 0
	If side Then t = k+4*n Else t = k
	ReDim coord(n,2)
	ReDim index(t-1)
	coord(n, 1) = h
	For i = 0 To n-1
		ang = 2*3.141592*i/n
		coord(i, 0) = r * Cos(ang)
		coord(i, 2) = r * Sin(ang)
		coord(i, 1) = -h
		If bottom Then index(i) = i
		If side Then
			index(k+4*i) = -1
			index(k+4*i+1) = i
			index(k+4*i+2) = i-1
			index(k+4*i+3) = n
		End If
	Next
	If side Then index(k+2) = n-1
End Sub

Sub Cylinder2Faceset (ByVal node, ByRef coord, ByRef index)
	Const n = 20
	h = node("height")/2
	r = node("radius")
	side = node("side")
	top = node("top")
	bottom = node("bottom")
	If side Then k = 5*n Else k = 0
	If top Then m = k+n+1 Else m = k
	If bottom Then t = m+n Else t = m
	ReDim coord(2*n,2)
	ReDim index(t-1)
	For i = 0 To n-1
		ang = 2*3.141592*i/n
		coord(i, 0) = r * Cos(ang)
		coord(i, 2) = r * Sin(ang)
		coord(i, 1) = -h
		coord(i+n, 0) = coord(i, 0)
		coord(i+n, 2) = coord(i, 2)
		coord(i+n, 1) = h
		If side Then
			index(5*i) = i
			index(5*i+1) = i-1
			index(5*i+2) = n+i-1
			index(5*i+3) = n+i
			index(5*i+4) = -1
		End If
		If top Then index(k+i) = 2*n-i-1
		If bottom Then index(m+i) = i
	Next
	If side Then
		index(1) = n-1
		index(2) = 2*n-1
	End If
	If top Then index(k+n) = -1
End Sub

Sub ConvertToFaceset
	Dim coord
	Dim index
	Set node = CurrentEntity
	If Not node Is Nothing Then
		If node.EntityType = vpNode Then
			If node.TypeName = "Box" Then
				Box2Faceset node, coord, index
			ElseIf node.TypeName = "Cone" Then
				Cone2Faceset node, coord, index
			ElseIf node.TypeName = "Cylinder" Then
				Cylinder2Faceset node, coord, index
			End If
		End If
	End If
	If Not IsArray(index) Then
		MsgBox "Please, select Box, Cone or Cylinder node"
		Exit Sub
	End If
	If node.References.Count > 0 Or _
			node.InRoutes.Count  > 0 Or _
			node.OutRoutes.Count > 0 Then
		If MsgBox("All references to the node will be deleted. Continue?",_
			vbOKCancel) = vbCancel Then Exit Sub
	End If
	Set owner = node.Owner
	If Not owner Is Nothing Then
		If owner.EntityType = vpField Then
			If owner.Type = vpfSFNode Then
				BeginOperation "Convert to Faceset"
				owner.Value = "IndexedFaceSet"
				Set node = owner.Value
				node("colorPerVertex") = False
				node("creaseAngle") = 1
				node("coord") = "Coordinate"
				node("coord")("point") = coord
				node("coordIndex") = index
				EndOperation
				Exit Sub
			End If
		End If
	End If
	MsgBox "Must be in a Shape node"
End Sub
